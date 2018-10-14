//
// Created by Rafiq on 2018-10-06.
//

#include <sstream>
#include <algorithm>
#include <vector>
#include <string>
#include "Parser.h"



Camera* build_camera(std::string line, std::stringstream &ss){
}


std::vector<std::pair<std::string, std::string>> tokenize(std::string scene_des){
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
            //std::cout << word << std::endl;
            tokens.emplace_back(std::make_pair("field", word));
        } else if (scene_des[i] == '['){
            tokens.emplace_back(std::make_pair("[", ""));
        } else if (scene_des[i] == ']'){
            tokens.emplace_back(std::make_pair("]", ""));
        } else if (scene_des[i] == '-'){
            tokens.emplace_back(std::make_pair("-", ""));
        } else if (scene_des[i] == '.'){
            tokens.emplace_back(std::make_pair(".", ""));
        }else if (isdigit(scene_des[i])){
            size_t start = i;
            ++i;
            while (i < scene_des.size() && isdigit(scene_des[i])){
                ++i;
            }
            word = scene_des.substr(start, i-start);
            tokens.emplace_back(std::make_pair("value", word));
            --i;
            //std::cout << word << std::endl;
        }else if (isalnum(scene_des[i])){
            size_t start = i;
            ++i;
            while ( i < scene_des.size() && isalnum(scene_des[i])){
                ++i;
            }
            word = scene_des.substr(start, i-start);
            --i;
            if (word == "LookAt"){
                tokens.emplace_back(std::make_pair(word, ""));
            }else if (word == "Camera"){
                tokens.emplace_back(std::make_pair(word, ""));
            }else if (word == "Sampler"){
                tokens.emplace_back(std::make_pair(word, ""));
            }else if (word == "Integrator"){
                tokens.emplace_back(std::make_pair(word, ""));
            }else if (word == "Film"){
                tokens.emplace_back(std::make_pair(word, ""));
            }else if (word == "WorldBegin"){
                tokens.emplace_back(std::make_pair(word, ""));
            }else if (word == "WorldEnd"){
                tokens.emplace_back(std::make_pair(word, ""));
            }else if (word == "AttributeBegin"){
                tokens.emplace_back(std::make_pair(word, ""));
            }else if (word == "AttributeEnd"){
                tokens.emplace_back(std::make_pair(word, ""));
            }else if (word == "Material"){
                tokens.emplace_back(std::make_pair(word, ""));
            }else if (word == "Shape"){
                tokens.emplace_back(std::make_pair(word, ""));
            }else if (word == "Translate"){
                tokens.emplace_back(std::make_pair(word, ""));
            }else if (word == "LightSource"){
                tokens.emplace_back(std::make_pair(word, ""));
            }else if (word == "Filter"){
                tokens.emplace_back(std::make_pair(word, ""));
            }else if (word == "Texture"){
                tokens.emplace_back(std::make_pair(word, ""));
            }else {
                std::cout << "Unknown Token: " << word << std::endl;
            }
        }
        ++i;
    }
    return tokens;
};

bool parse_num(std::vector<std::pair<std::string, std::string>>::iterator &start,
               std::vector<std::pair<std::string, std::string>>::iterator &end, float &f){

    if ( start == end ){
        return false;
    }
    std::string retval;
    if ( start->first == "-" ){
        retval += "-";
        ++start;
        if (start == end){
            return false;
        }
    }
    std::string before_dec;
    std::string after_dec;
    if (start->first == "value"){
        before_dec = start->second;
        ++start;
        if(start == end || start->first != "."){
            // not a floating point number
            retval += before_dec;
            f = float(atof(retval.c_str()));
            return true;
        }
    } else if (start->first != "."){
        return false;
    }

    ++start;
    if (start == end){
        return false;
    }
    // is a floating point number
    if (start->first == "value"){
        after_dec = start->second;
        retval += before_dec + "." + after_dec;
        f = float(atof(retval.c_str()));
        ++start;
        return true;
    }
    return false;

}

bool parse_lookat(std::vector<std::pair<std::string, std::string>>::iterator &start,
                  std::vector<std::pair<std::string, std::string>>::iterator &end,
                  Vector3f &look_from, Vector3f &look_at, Vector3f &vup){

    if ( start == end ){
        return false;
    }

    if ( start->first != "LookAt" ){
        return false;
    }

    ++start;

    float x, y, z;
    bool xp, yp, zp;
    xp = parse_num(start, end, x);
    yp = parse_num(start, end, y);
    zp = parse_num(start, end, z);
    if (!xp || !yp || !zp){
        std::cout << "parsing error in LookAt->look from block" << std::endl;
        return false;
    }
    look_from = Vector3f(x, y, z);

    xp = parse_num(start, end, x);
    yp = parse_num(start, end, y);
    zp = parse_num(start, end, z);
    if (!xp || !yp || !zp){
        std::cout << "parsing error in LookAt->look at block" << std::endl;
        return false;
    }
    look_at = Vector3f(x, y, z);

    xp = parse_num(start, end, x);
    yp = parse_num(start, end, y);
    zp = parse_num(start, end, z);
    if (!xp || !yp || !zp){
        std::cout << "parsing error in LookAt->vup block" << std::endl;
        return false;
    }
    vup = Vector3f(x, y, z);
    return true;
}


bool parse_cam(std::vector<std::pair<std::string, std::string>>::iterator &start,
               std::vector<std::pair<std::string, std::string>>::iterator &end, Camera **c,
               const Vector3f &lookfrom, const Vector3f &lookat, const Vector3f &vup){

    if ( start == end ){
        return false;
    }

    if ( start->first != "Camera" ){
        return false;
    }

    ++start;
    if ( start == end ){
        return false;
    }

    if ( start->first != "field" ){
        return false;
    }

    float shutteropen = 0;
    float shutterclose = 1;
    float x = nx;
    float y = ny;
    float frameaspectratio = nx/ny;
    float screenwindow;
    float lensradius = 0;
    float focaldistance = MAXFLOAT;
    float fov = 90;
    float halffov = fov / 2;
    std::string lensfile;
    float aperturediameter = 1.0f;
    float focusdistance = 10.0;
    bool simpleweighting = true;

    std::string cam_type = start->second;
    ++start;
    for(;  start->first == "field";){
        std::string field = start->second;
        size_t pos = field.find(' ');
        if ( pos == std::string::npos ){
            std::cout << "invalid values: " << field << std::endl;
            return false;
        }
        std::string type = field.substr(0, pos);
        std::string field_name = field.substr(pos+1);
        std::transform(type.begin(), type.end(), type.begin(), tolower);

        ++start;
        if ( start == end ){
            std::cout << "unexpected end of tokens" << std::endl;
            return false;
        }

        if ( start->first == "[" ){
            ++start;
            if ( start == end ){
                std::cout << "unexpected end of tokens" << std::endl;
                return false;
            }
        }

        if ( type == "integer" || type == "float" ){

            float num;
            if (!parse_num(start, end, num)){
                std::cout << "Incorrect Syntax in parsing camera, expected number for field: " << field_name << std::endl;
                return false;
            }

            if (field_name == "shutteropen"){
                shutteropen = num;
            }else if (field_name == "shutterclose"){
                shutterclose = num;
            }else if (field_name == "shutterclose"){
                shutterclose = num;
            }else if (field_name == "frameaspectratio"){
                frameaspectratio = num;
            }else if (field_name == "screenwindow"){
                screenwindow = num;
            }else if (field_name == "lensradius"){
                lensradius = num;
            }else if (field_name == "focaldistance"){
                focaldistance = num;
            }else if (field_name == "fov"){
                fov = num;
            }else if (field_name == "halffov"){
                halffov = num;
            }else if (field_name == "aperturediameter"){
                aperturediameter = num;
            }else if (field_name == "focusdistance"){
                focusdistance = num;
            }else {
                std::cout << "Unknown field name in camera: " << field_name << std::endl;
            }

        } else if ( type == "point3" || type == "vector3" || type == "normal3" ||
                type == "point" || type == "vector" || type == "normal"){

        } else if ( type == "point2" || type == "vector2" ){

        } else if ( type == "spectrum " ){

        } else if ( type == "bool" ){

        } else {
            std::cout << "Unknown Type: " << type << std::endl;
            return false;
        }

        if ( start != end && start->first == "]" ){
            ++start;
        }
        std::cout << "-" << std::endl;
    }

    *c = new Camera(lookfrom, lookat, vup, fov, frameaspectratio, aperturediameter, focusdistance, shutteropen, shutterclose);
    return true;
}


bool parse_sampler(std::vector<std::pair<std::string, std::string>>::iterator &start,
                   std::vector<std::pair<std::string, std::string>>::iterator &end){

    if ( start == end ){
        return false;
    }

    if ( start->first != "Sampler" ){
        return false;
    }

    ++start;
    if ( start == end ){
        return false;
    }

    if ( start->first != "field" ){
        return false;
    }


    std::string sampler = std::string(start->second);
    ++start;
    if ( start == end ){
        std::cout << "Unexpected end of tokens, expected sample per pixel for sampler" << std::endl;
        return false;
    }
    int pixelsamples;
    if ( start->first != "field"){
        std::cout << "Unexpected token: " << start->first << " was expecting field" << std::endl;
        return false;
    }
    ++start;

    if ( start == end){
        std::cout << "Unexpected end of tokens, expected sample per pixel for sampler" << std::endl;
        return false;
    }

    if ( start->first != "value" ){
        std::cout << "Unexpected token: " << start->first << " was expecting value" << std::endl;
        return false;

    }

    pixelsamples = std::atoi(start->second.c_str());
    ++start;
    return true;
}

bool parse_integrator(std::vector<std::pair<std::string, std::string>>::iterator &start,
                   std::vector<std::pair<std::string, std::string>>::iterator &end){

    if ( start == end ){
        return false;
    }

    if ( start->first != "Integrator" ){
        return false;
    }

    ++start;
    if ( start == end ){
        return false;
    }

    if ( start->first != "field" ){
        return false;
    }


    std::string integrator = std::string(start->second);
    ++start;
    return true;
}


bool parse_light(std::vector<std::pair<std::string, std::string>>::iterator &start,
                 std::vector<std::pair<std::string, std::string>>::iterator &end,
                 Hitable **lights){

    ++start;
    if (start == end){
        return false;
    }



    return true;
}

bool parse_world(std::vector<std::pair<std::string, std::string>>::iterator &start,
                 std::vector<std::pair<std::string, std::string>>::iterator &end,
                 Hitable **lights, Hitable **world){

    if ( start == end ){
        return false;
    }

    if ( start->first != "WorldBegin" ){
        return false;
    }

    ++start;
    while ( start != end && start->first != "WorldEnd" ){

        if (start->first == "LightSource"){

        }else if (start->first == "AttributeBegin"){

        }

    }

    return true;
}

bool parse_pbrt_scene(std::string scene_des, Hitable **lights, Camera **cam, Hitable **world){
    std::string word;
    std::stringstream ss(scene_des);
    std::string cleaned;
    std::vector<std::pair<std::string, std::string>> tokens;
    size_t i = 0;

    std::cout << scene_des << std::endl;
    tokens = tokenize(scene_des);
    for (auto &t : tokens){
        //std::cout << t.first << ", " << t.second<< std::endl;
    }
    auto start = tokens.begin();
    auto end = tokens.end();
    Vector3f look_at;
    Vector3f look_from;
    Vector3f vup;
    if (!parse_lookat(start, end, look_at, look_from, vup)){
        return false;
    }

    std::cout << "lookfrom: " << look_from << "\nlookat: " << look_at << "\nvup: " << vup << std::endl;
    if (!parse_cam(start, end, cam, look_from, look_at, vup)){
        std::cout << "error parsing camera" << std::endl;
        return false;
    }
    std::cout << "parsed camera successfully" << std::endl;

    if (!parse_sampler(start, end)){
        std::cout << "error parsing sampler" << std::endl;
        return false;
    }
    std::cout << "parsed sampler successfully" << std::endl;

    if (!parse_integrator(start, end)){
        std::cout << "error parsing integrator" << std::endl;
        return false;
    }
    std::cout << "parsed integrator successfully" << std::endl;



    return true;
}


void parseSceneFile(const std::vector<std::string> &sceneDes,
                    std::vector<Triangle> &geoList,
                    std::vector<Light> &lightList,
                    Camera** cam){

    std::vector<Point3f> vertexList;
    std::vector<Vector3f> textureList;
    std::vector<Vector3f> normalList;

    float x, y, z, x2, y2, z2, x3, y3, z3, x4, y4, z4;
    std::string c;
    char delim;
    unsigned i = 0;
    for(const auto &s: sceneDes){
        std::stringstream sstream(s);

        if(sstream >> c){
            if(c == "light"){
                std::string name;
                float intensity, r, g, b, tx, ty, tz, sx, sy, sz, rx, ry, rz;
                if(sstream >> intensity >> r >> g >> b >> tx >> ty >> tz >> sx >> sy >> sz >> rx >> ry >> rz){
                    Light l = Light(name,
                                    intensity,
                                    r,g,b,
                                    tx,ty,tz,
                                    rx,ry,rz,
                                    sx,sy,sz);
                    lightList.push_back(l);
                }
            }

            if(c == "cam"){
                float tx, ty, tz, sx, sy, sz, rx, ry, rz, focalLength;
                if(sstream >> tx >> ty >> tz >> sx >> sy >> sz >> rx >> ry >> rz >> focalLength){

//                    *cam = new Camera(Vector3f(tx, ty, tz),
//                                      Vector3f(rx, ry, rz),
//                                      Vector3f(sx, sy, sz), focalLength, 90, float(nx)/float(ny));
                }
            }

            if(c == "v"){
                if(sstream >> x >> y >> z){
                    vertexList.emplace_back(Point3f(x, y, z));
                }
            }
            if(c == "vt"){
                if(sstream >> x >> y){
                    textureList.emplace_back(Vector3f(x, y, 0));
                }
            }
            if(c == "vn"){
                if(sstream >> x >> y >> z){
                    normalList.emplace_back(Vector3f(x, y, z));
                }
            }
            if(c == "f"){
                if(sstream >> x >> delim >> x2 >> delim >> x3
                           >> y >> delim >> y2 >> delim >> y3
                           >> z >> delim >> z2 >> delim >> z3){

                    if(sstream >> x4 >> delim >> y4 >> delim >> z4){

                        Point3f p1 = vertexList[x];
                        Point3f p2 = vertexList[y];
                        Point3f p3 = vertexList[z];
                        Point3f p4 = vertexList[x4];

                        Triangle tri1 = Triangle(p1, p2, p3,
                                                 textureList[x2], textureList[y2], textureList[z2],
                                                 normalList[x3], normalList[y3], normalList[z3]);

                        geoList.push_back(tri1);

                        if(p4.x != p1.x && p4.x != p1.y && p4.x != p1.z &&
                           p4.y != p1.x && p4.y != p1.y && p4.y != p1.z &&
                           p4.z != p1.x && p4.z != p1.y && p4.z != p1.z){
                            Triangle tri2 = Triangle(p4, p2, p3,
                                                     textureList[y2], textureList[z2], textureList[y4],
                                                     normalList[y3], normalList[z3], normalList[z4]);
                            geoList.push_back(tri2);
                        }else if(p4.x != p2.x && p4.x != p2.y && p4.x != p2.z &&
                                 p4.y != p2.x && p4.y != p2.y && p4.y != p2.z &&
                                 p4.z != p2.x && p4.z != p2.y && p4.z != p2.z){

                            Triangle tri2 = Triangle(p4, p1, p3,
                                                     textureList[x2], textureList[z2], textureList[y4],
                                                     normalList[x3], normalList[z3], normalList[z4]);
                            geoList.push_back(tri2);
                        }else{
                            Triangle tri2 = Triangle(p4, p1, p2,
                                                     textureList[x2], textureList[y2], textureList[y4],
                                                     normalList[x3], normalList[y3], normalList[z4]);
                            geoList.push_back(tri2);
                        }



                    }else{

                        Triangle tri = Triangle(vertexList[x], vertexList[y], vertexList[z],
                                                textureList[x2], textureList[y2], textureList[z2],
                                                normalList[x3], normalList[y3], normalList[z3]);
                        geoList.push_back(tri);
                    }
                }
            }
        }


    }
}