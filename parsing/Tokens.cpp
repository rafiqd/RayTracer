//
// Created by Rafiq on 2018-10-28.
//

#include "Tokens.h"

void
tokenize(std::string scene_des, TokenList &token_list){
    std::string word;
    std::vector<std::pair<std::string, std::string>> tokens;
    size_t i = 0;
    while ( i < scene_des.size() ){

        if (scene_des[i] == '#'){
            while (i < scene_des.size() && scene_des[i] != '\n'){
                ++i;
            }
        }

        if (scene_des[i] == '"'){
            size_t start = i;
            ++i;
            while(i < scene_des.size() && scene_des[i] != '"'){
                ++i;
            }
            word = scene_des.substr(start+1, i-start-1);
            token_list.addFieldToken(word);
        } else if (scene_des[i] == '['){
            token_list.addReserveToken("[");
        } else if (scene_des[i] == ']'){
            token_list.addReserveToken("]");
        } else if (scene_des[i] == '-'){
            token_list.addReserveToken("-");
        } else if (scene_des[i] == '.'){
            token_list.addReserveToken(".");
        }else if (isdigit(scene_des[i])){
            size_t start = i;
            ++i;
            while (i < scene_des.size() && isdigit(scene_des[i])){
                ++i;
            }
            word = scene_des.substr(start, i-start);
            token_list.addValueToken(word);
            --i;
        }else if (isalnum(scene_des[i])){
            size_t start = i;
            ++i;
            while ( i < scene_des.size() && isalnum(scene_des[i])){
                ++i;
            }
            word = scene_des.substr(start, i-start);
            --i;
            if (word == "LookAt"){
                token_list.addReserveToken("LookAt");
            }else if (word == "Camera"){
                token_list.addReserveToken("Camera");
            }else if (word == "Sampler"){
                token_list.addReserveToken("Sampler");
            }else if (word == "Integrator"){
                token_list.addReserveToken("Integrator");
            }else if (word == "Film"){
                token_list.addReserveToken("Film");
            }else if (word == "WorldBegin"){
                token_list.addReserveToken("WorldBegin");
            }else if (word == "WorldEnd"){
                token_list.addReserveToken("WorldEnd");
            }else if (word == "AttributeBegin"){
                token_list.addReserveToken("AttributeBegin");
            }else if (word == "AttributeEnd"){
                token_list.addReserveToken("AttributeEnd");
            }else if (word == "Material"){
                token_list.addReserveToken("Material");
            }else if (word == "Shape"){
                token_list.addReserveToken("Shape");
            }else if (word == "Translate"){
                token_list.addReserveToken("Translate");
            }else if (word == "LightSource"){
                token_list.addReserveToken("LightSource");
            }else if (word == "Filter"){
                token_list.addReserveToken("Filter");
            }else if (word == "Texture"){
                token_list.addReserveToken("Texture");
            }else {
                std::cout << "Unknown Token: " << word << std::endl;
            }
        }
        ++i;
    }
}