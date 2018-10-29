//
// Created by Rafiq on 2018-10-06.
//

#include <sstream>
#include <algorithm>
#include <vector>
#include "Parser.h"
#include "ParsingException.h"
#include "../Material.h"
#include "../Sphere.h"


float
parse_num(TokenList &tokenList){

    std::string retval;
    const Token *t = tokenList.getNextToken();
    if ( t->getValue() == "-" ){
        retval += "-";
        t = tokenList.getNextToken();
    }
    std::string before_dec;
    std::string after_dec;
    if (t->type() == "value"){
        before_dec = t->getValue();
        t = tokenList.peakNextToken();
        if(t->getValue() != "."){
            // not a floating point number
            retval += before_dec;
            return atof(retval.c_str());
        }
        t = tokenList.getNextToken();
    }

    if (t->getValue() != "."){
        std::stringstream ss;
        ss << "Expected '.' , got: " << t->getValue() << std::endl;
        throw ParsingException(ss.str());
    }

    t = tokenList.getNextToken();

    // is a floating point number
    if (t->type() != "value"){
        std::stringstream ss;
        ss << "Expected a value , got: " << t->getValue() << std::endl;
        throw ParsingException(ss.str());
    }

    after_dec = t->getValue();
    retval += before_dec + "." + after_dec;
    return atof(retval.c_str());

}


Vector3f
parse_vector3(TokenList &tokenList){
    float x = parse_num(tokenList);
    float y = parse_num(tokenList);
    float z = parse_num(tokenList);
    return Vector3f{x ,y, z};
}


std::array<Vector3f,3>
parse_lookat(TokenList& tokenList){

    const std::string tok = tokenList.getNextToken()->getValue();
    if ( tok != "LookAt" ){
        std::stringstream ss;
        ss << "Expected LookAt, got: " << tok << std::endl;
        throw ParsingException(ss.str());
    }

    return std::array<Vector3f, 3>{parse_vector3(tokenList), parse_vector3(tokenList), parse_vector3(tokenList)};
}


Camera
parse_cam(TokenList &tokenList, const std::array<Vector3f, 3> &look){

    const Token *t = tokenList.getNextToken();

    if ( t->getValue() != "Camera" ){
        std::stringstream ss;
        ss << "Expected 'Camera', got: " << t->getValue() << std::endl;
        throw ParsingException(ss.str());
    }

    t = tokenList.getNextToken();
    if ( t->type() != "field" ){
        std::stringstream ss;
        ss << "Expected a 'field', got: " << t->getValue() << std::endl;
        throw ParsingException(ss.str());
    }

    /*
     * some of these aren't implemented in my renderer yet, but these are possible values of PBRT cameras
     */
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

    const std::string cam_type = t->getValue();

    while( tokenList.peakNextToken()->type() == "field" ){
        t = tokenList.getNextToken();
        const std::string &field = t->getValue();
        size_t pos = field.find(' ');
        if ( pos == std::string::npos ){
            std::stringstream ss;
            ss << "Invalid Values, got: " << field << std::endl;
            throw ParsingException(ss.str());
        }

        std::string type = field.substr(0, pos);
        const std::string field_name = field.substr(pos+1);
        std::transform(type.begin(), type.end(), type.begin(), tolower);

        if (tokenList.peakNextToken()->getValue() == "[" ){
            t = tokenList.getNextToken();
        }

        if ( type == "integer" || type == "float" ){

            float num = parse_num(tokenList);

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
            // not implemented yet
        } else if ( type == "point2" || type == "vector2" ){
            // not implemented yet
        } else if ( type == "spectrum " ){
            // not implemented yet
        } else if ( type == "bool" ){
            // not implemented yet
        } else {
            std::stringstream ss;
            ss << "Unknown Type, got: " << type << std::endl;
            throw ParsingException(ss.str());
        }

        if ( t->getValue() == "]" ){
            t = tokenList.getNextToken();
        }
    }

    return Camera(look[0], look[1], look[2], fov, frameaspectratio, aperturediameter, focusdistance, shutteropen, shutterclose);
}


void
parse_sampler(TokenList &tokenList){

    const Token *t = tokenList.getNextToken();
    if ( t->getValue() != "Sampler" ){
        std::stringstream ss;
        ss << "Expected 'Sampler', got: " << t->getValue() << std::endl;
        throw ParsingException(ss.str());
    }

    t = tokenList.getNextToken();
    if ( t->type() != "field" ){
        std::stringstream ss;
        ss << "Expected 'field type', got: " << t->getValue() << std::endl;
        throw ParsingException(ss.str());
    }


    const std::string sampler = t->getValue();

    t = tokenList.getNextToken();
    if ( t->type() != "field" ){
        std::stringstream ss;
        ss << "Expected 'field type', got: " << t->getValue() << std::endl;
        throw ParsingException(ss.str());
    }
    t = tokenList.getNextToken();

    if ( t->type() != "value" ){
        std::stringstream ss;
        ss << "Expected 'value type', got: " << t->getValue() << std::endl;
        throw ParsingException(ss.str());
    }

    int pixel_samples = std::atoi(t->getValue().c_str());
    std::cout << 'pixel samples ' << pixel_samples << std::endl;
    // TODO: implement different samplers
}


void
parse_integrator(TokenList &tokenList){

    const Token *t = tokenList.getNextToken();
    if ( t->getValue() != "Integrator" ){
        std::stringstream ss;
        ss << "Expected 'Integrator', got: " << t->getValue() << std::endl;
        throw ParsingException(ss.str());
    }

    t = tokenList.getNextToken();
    if ( t->type() != "field" ){
        std::stringstream ss;
        ss << "Expected 'field type', got: " << t->getValue() << std::endl;
        throw ParsingException(ss.str());
    }


    const std::string integrator = t->getValue();
    std::cout << "Integrator: " << integrator << std::endl;
    // TODO: implement different integrators

}


void
parse_light(TokenList &tokenList, std::vector<Hitable*> &lights, int nLights){

    // TODO: implement all these light types
    const Token *t = tokenList.getNextToken();

    if (t->type() != "field" ){
        std::stringstream ss;
        ss << "Expected 'field type', got: " << t->getValue() << std::endl;
        throw ParsingException(ss.str());
    }

    const std::string light_type = t->getValue();

    t = tokenList.getNextToken();
    if ( t->type() != "field"){
        std::stringstream ss;
        ss << "Expected 'field type', got: " << t->getValue() << std::endl;
        throw ParsingException(ss.str());
    }

    t = tokenList.getNextToken();
    t = tokenList.getNextToken();

    if ( light_type == "distant"){

    }else if ( light_type == "goniometric"){
        parse_vector3(tokenList);

    }else if ( light_type == "infinite"){
        Material *l = new DiffuseLight(new ConstantTexture(parse_vector3(tokenList)));
        lights.emplace_back(new FlipNormals(new Sphere(Vector3f(0,0,0), 10000, l)));

    }else if ( light_type == "point"){

    }else if ( light_type == "projection"){

    }else if ( light_type == "spot"){

    }else {
        std::stringstream ss;
        ss << "Unknown light type , got: " << light_type << std::endl;
        throw ParsingException(ss.str());
    }


}


void
parse_world(TokenList &tokenList, Hitable **lights, Hitable **world){

    const Token *t = tokenList.getNextToken();
    if ( t->getValue() != "WorldBegin" ){
        std::stringstream ss;
        ss << "Expected 'WorldBegin', got: " << t->getValue() << std::endl;
        throw ParsingException(ss.str());
    }

    int nLights = 0;
    std::vector<Hitable*> parsed_lights;
    while ( tokenList.peakNextToken()->getValue() != "WorldEnd" ){
        // TODO: add parsing for a pbrt 'world'
        t = tokenList.getNextToken();
        if ( t->getValue() == "LightSource"){
            parse_light(tokenList, parsed_lights, nLights);
        }else if ( t->getValue() == "AttributeBegin"){
        }
    }
}


bool
parse_pbrt_scene(const std::string &scene_des, Hitable **lights, Camera **cam, Hitable **world){

    TokenList tokens;
    tokenize(scene_des, tokens); // have to pass this in by reference because the copy ctor is deleted

    try{
        std::array<Vector3f, 3> look = parse_lookat(tokens);
        Camera render_cam = parse_cam(tokens, look);
        parse_sampler(tokens);
        parse_integrator(tokens);
        parse_world(tokens, lights, world);
    } catch (int err){
        std::cerr << "error parsing look at" << std::endl;
        return false;
    } catch (ParsingException &pe){
        std::cerr << "Error in parsing: " << pe.what() << std::endl;
        return false;
    }

    return true;
}

[[deprecated("this was to parse my own scenes using alembic caches, i switched to reading pbrt scenes instead")]]
void
parseSceneFile(const std::vector<std::string> &sceneDes,
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