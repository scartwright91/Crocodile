#include "Project.h"

Project::Project(std::string name, std::string path, bool loadProjectFromFile)
{
    this->name = name;
    this->path = path;
    if (loadProjectFromFile)
        load();
}

void Project::save(LevelData ld)
{
    Json::Value data;

    // project data
    data["project"]["name"] = name;
    data["project"]["path"] = path;

    // level data TODO: iterate over all levels
    Json::Value canvasSize(Json::arrayValue);
    canvasSize.append((int)ld.canvasSize.x);
    canvasSize.append((int)ld.canvasSize.y);
    data["levels"][ld.name]["canvas_size"] = canvasSize;

    Json::Value layerKeys(Json::arrayValue);
    Json::Value entityKeys(Json::arrayValue);
    Json::Value textureKeys(Json::arrayValue);

    // layers
    for (s2d::Layer *layer : ld.layers)
        layerKeys.append(layer->name);

    // entities
    for (EntityData ed : ld.entitiesData)
    {
        entityKeys.append(ed.label);
        data["levels"][ld.name]["entity_data"][ed.label] = ed.texture.name;
    }

    // textures
    Json::Value textures(Json::arrayValue);
    for (ResourceManager::TextureData td : ld.textures)
    {
        textureKeys.append(td.name);
        data["levels"][ld.name]["textures"][td.name] = td.path;
    }

    data["levels"][ld.name]["layers"] = layerKeys;
    data["levels"][ld.name]["entity_keys"] = entityKeys;
    data["levels"][ld.name]["texture_keys"] = textureKeys;

    // TODO implement placed entities (add serialise to Object class)

    // write to file
    std::ofstream file_id;
    file_id.open("level_data.cld");
    Json::StyledWriter styledWriter;
    file_id << styledWriter.write(data);
    file_id.close();
}

void Project::load()
{
    // read project from file
    std::ifstream f("level_data.cld");
    Json::Value data;
    f >> data;

    std::cout << data << std::endl;
}

void Project::undo()
{
}

void Project::redo()
{
}
