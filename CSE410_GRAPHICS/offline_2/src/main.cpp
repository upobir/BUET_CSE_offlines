#include <iostream>

#include <string>
#include <fstream>

#include "GraphicsProcessor.hpp"
#include "bitmap_image.hpp"

int main(int argc, char** argv){
    
    std::string sceneFilename = "scene.txt";
    std::string configFilename = "config.txt";

    if(argc > 2)
        sceneFilename = std::string(argv[1]);
    if(argc > 3)
        configFilename = std::string(argv[2]);

    std::ifstream sceneFile(sceneFilename);
    std::ifstream configFile(configFilename);

    std::ofstream stage1File("stage1.txt");
    std::ofstream stage2File("stage2.txt");
    std::ofstream stage3File("stage3.txt");
    std::ofstream zbufferFile("z_buffer.txt");
    bitmap_image bmp;

    GraphicsProcessor processor(sceneFile, configFile, stage1File, stage2File, stage3File, zbufferFile, bmp);

    

    processor.processTransformations();
    processor.processDrawing();

    sceneFile.close();
    configFile.close();
    stage1File.close();
    stage2File.close();
    stage3File.close();
    zbufferFile.close();
    bmp.save_image("out.bmp");

    return 0;
}