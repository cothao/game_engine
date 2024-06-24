#include "model.h"
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model() {}

Model::Model(string const& path, bool flip, glm::vec3 pos, glm::vec3 scale, bool gamma)
    : flip(flip), gammaCorrection(gamma), Position(pos), Scale(scale), ID(id_generator++) {
    loadModel(path);
}

void Model::Draw(Shader& shader, glm::mat4 projection, glm::mat4 view, glm::vec3 camera, glm::vec3 lightPos) {
    this->LightPosition = lightPos;

    shader.use();

    glm::mat4 model = glm::mat4(1.0);

    shader.setVec3("viewPos", camera.x, camera.y, camera.z);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    model = glm::translate(model, this->Position);
    model = glm::rotate(model, this->RotateX, this->RotateDirX);
    model = glm::rotate(model, this->RotateY, this->RotateDirY);
    model = glm::rotate(model, this->RotateZ, this->RotateDirZ);
    model = glm::scale(model, this->Scale);

    shader.setVec3("lightPos", this->LightPosition.x, this->LightPosition.y, this->LightPosition.z);
    shader.setVec3("color", this->Color.r, this->Color.g, this->Color.b);
    shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    shader.setMat4("model", model);

    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}

void Model::loadModel(string const& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    if (mesh->HasBones()) {
        for (int i = 0; i < mesh->mNumBones; i++) {
            std::cout << mesh->mBones[i] << "\n";
        }
    }

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;

            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;

            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, scene, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, scene, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, scene, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, scene, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const aiScene* scene, string typeName) {
    vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        std::cout << "Loading texture: " << str.C_Str() << std::endl;

        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++) {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                std::cout << "Texture already loaded: " << str.C_Str() << std::endl;
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip) {
            Texture texture;
            if (str.C_Str()[0] == '*') {
                int textureIndex = std::atoi(&str.C_Str()[1]);
                aiTexture* aiTex = scene->mTextures[textureIndex];

                if (aiTex->mHeight == 0) {
                    int width, height, nrComponents;
                    unsigned char* data = stbi_load_from_memory(
                        reinterpret_cast<unsigned char*>(aiTex->pcData),
                        aiTex->mWidth,
                        &width,
                        &height,
                        &nrComponents,
                        0
                    );
                    if (data) {
                        texture.id = TextureFromData(data, width, height, nrComponents, this->flip);
                        stbi_image_free(data);
                    }
                } else {
                    texture.id = TextureFromData(
                        reinterpret_cast<unsigned char*>(aiTex->pcData),
                        aiTex->mWidth,
                        aiTex->mHeight,
                        4,
                        this->flip
                    );
                }
                texture.path = str.C_Str();
            } else {
                texture.id = TextureFromFile(str.C_Str(), this->directory, this->flip, this->gammaCorrection);
            }
            if (texture.id != 0) {
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);
                std::cout << "Texture loaded: " << str.C_Str() << std::endl;
            } else {
                std::cerr << "Failed to load texture: " << str.C_Str() << std::endl;
            }
        }
    }
    return textures;
}

std::string Model::getPosition() {
    return "{" + std::to_string(this->Position.x) + ',' + std::to_string(this->Position.y) + ',' + std::to_string(this->Position.z) + "}";
}

unsigned int TextureFromFile(const char* path, const string& directory, bool flip, bool gamma) {
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    stbi_set_flip_vertically_on_load(flip);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

unsigned int TextureFromData(unsigned char* data, int width, int height, int nrComponents, bool flip) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    GLenum format;
    if (nrComponents == 1)
        format = GL_RED;
    else if (nrComponents == 3)
        format = GL_RGB;
    else if (nrComponents == 4)
        format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return textureID;
}
