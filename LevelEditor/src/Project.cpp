#include "Project.h"

Project::Project(
    std::string name,
    std::string path,
    bool loadProjectFromFile,
    ResourceManager *rm) : name(name), path(path), loadProjectFromFile(loadProjectFromFile), rm(rm)
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
    Json::Value canvasColor(Json::arrayValue);

    Json::Value layerKeys(Json::arrayValue);
    Json::Value entityKeys(Json::arrayValue);
    Json::Value textureKeys(Json::arrayValue);
    Json::Value placedEntities(Json::arrayValue);

    // layers
    for (s2d::Layer *layer : ld.layers)
        layerKeys.append(layer->name);

    // textures
    Json::Value textures(Json::arrayValue);
    for (ResourceManager::TextureData td : ld.textures)
    {
        textureKeys.append(td.name);
        data["levels"][ld.name]["textures"][td.name] = td.path;
    }

    // entities
    for (EntityData *ed : ld.entitiesData)
    {
        entityKeys.append(ed->label);
        data["levels"][ld.name]["entity_data"][ed->label] = ed->texture.name;
    }

    // placed entities
    for (SceneEntityData *sed : ld.sceneEntityData)
    {
        Json::Value entData;
        entData["label"] = sed->label;
        entData["layer"] = sed->layer;
        entData["rotation"] = sed->rotation;
        entData["alpha"] = sed->alpha;
        Json::Value pos(Json::arrayValue);
        pos.append(sed->pos.x);
        pos.append(sed->pos.y);
        entData["pos"] = pos;
        Json::Value size(Json::arrayValue);
        size.append(sed->size.x);
        size.append(sed->size.y);
        entData["size"] = size;
        entData["texture"] = sed->texture;
        placedEntities.append(entData);
    }

    data["levels"][ld.name]["layers"] = layerKeys;
    data["levels"][ld.name]["texture_keys"] = textureKeys;
    data["levels"][ld.name]["entity_keys"] = entityKeys;
    data["levels"][ld.name]["placed_entity_keys"] = placedEntities;

    // write to file
    std::ofstream file_id;
    std::string projectPath = "";
    if (loadProjectFromFile)
        projectPath = path;
    else
        projectPath = path + ".cld";
    file_id.open(projectPath);
    Json::StyledWriter styledWriter;
    file_id << styledWriter.write(data);
    file_id.close();
}

LevelData Project::load()
{
    // read project from file
    std::ifstream f(path);
    Json::Value data;
    f >> data;

    // unpack into level data struct
    LevelData ld;

    // canvas
    const Json::Value canvasSizeData = data["levels"][ld.name]["canvas_size"];
    ld.canvasSize = glm::vec2(canvasSizeData[0].asFloat(), canvasSizeData[1].asFloat());

    // layers
    const Json::Value levelLayersData = data["levels"][ld.name]["layers"];
    std::vector<s2d::Layer *> layers = {};
    for (int i = 0; i < levelLayersData.size(); i++)
    {
        std::string layerName = levelLayersData[i].asString();
        layers.push_back(new s2d::Layer(layerName));
    }
    ld.layers = layers;

    // textures
    const Json::Value levelTexturesData = data["levels"][ld.name]["texture_keys"];
    std::vector<ResourceManager::TextureData> textures = {};
    for (int i = 0; i < levelTexturesData.size(); i++)
    {
        std::string textureName = levelTexturesData[i].asString();
        std::string texturePath = data["levels"][ld.name]["textures"][textureName].asString();
        rm->loadTexture(texturePath.c_str(), textureName, false);
        textures.push_back(rm->getTexture(textureName));
    }

    // entities data
    const Json::Value levelEntitiesData = data["levels"][ld.name]["entity_keys"];
    std::vector<EntityData *> entitiesData = {};
    for (int i = 0; i < levelEntitiesData.size(); i++)
    {
        std::string entityName = levelEntitiesData[i].asString();
        std::string entityTexture = data["levels"][ld.name]["entity_data"][entityName].asString();
        EntityData *ed = new EntityData();
        ed->label = entityName;
        ResourceManager::TextureData td = rm->getTexture(entityTexture);
        ed->texture = td;
        ed->size = glm::vec2(td.width, td.height);
        ed->colour = glm::vec3(0.f);
        entitiesData.push_back(ed);
    }

    // placed entities data
    const Json::Value placedEntitiesData = data["levels"][ld.name]["placed_entity_keys"];
    std::vector<SceneEntityData *> sceneEntitiesData = {};
    for (int i = 0; i < placedEntitiesData.size(); i++)
    {
        Json::Value entData = placedEntitiesData[i];
        SceneEntityData *sceneEnt = new SceneEntityData();
        sceneEnt->label = entData["label"].asString();
        sceneEnt->layer = entData["layer"].asString();
        sceneEnt->rotation = entData["rotation"].asFloat();
        sceneEnt->alpha = entData["alpha"].asFloat();
        sceneEnt->pos = glm::vec2(entData["pos"][0].asFloat(), entData["pos"][1].asFloat());
        sceneEnt->size = glm::vec2(entData["size"][0].asFloat(), entData["size"][1].asFloat());
        sceneEnt->texture = entData["texture"].asString();
        sceneEntitiesData.push_back(sceneEnt);
    }

    ld.entitiesData = entitiesData;
    ld.textures = textures;
    ld.sceneEntityData = sceneEntitiesData;

    // std::cout << data << std::endl;
    return ld;
}

void Project::undo()
{
}

void Project::redo()
{
}
