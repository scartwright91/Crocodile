#include "Project.h"

Project::Project(std::string name, std::string path, bool loadProjectFromFile) : name(name), path(path), loadProjectFromFile(loadProjectFromFile)
{
    if (loadProjectFromFile)
        data = load();
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

LevelData Project::load()
{
    // read project from file
    std::ifstream f("level_data.cld");
    Json::Value data;
    f >> data;

    // unpack into level data struct
    LevelData ld;

    // canvas
    const Json::Value canvasSizeData = data["levels"][ld.name]["canvas_size"];
    ld.canvasSize = glm::vec2(canvasSizeData[0].asFloat(), canvasSizeData[1].asFloat());

    // layers
    const Json::Value layersData = data["levels"][ld.name]["layers"];
    std::vector<s2d::Layer *> layers = {};
    for (int i = 0; i < layersData.size(); i++)
    {
        std::string layerName = layersData[i].asString();
        layers.push_back(new s2d::Layer(layerName));
    }
    ld.layers = layers;

    std::vector<EntityData> entitiesData = {};
    std::vector<ResourceManager::TextureData> textures = {};

    ld.entitiesData = entitiesData;
    ld.textures = textures;

    std::cout << data << std::endl;
    return ld;
}

void Project::undo()
{
}

void Project::redo()
{
}
