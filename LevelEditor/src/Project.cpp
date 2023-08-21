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

void Project::save(WorldData wd)
{
    Json::Value data;
    // project data
    data["project"]["name"] = name;
    data["project"]["path"] = path;

    // writing level data
    Json::Value levelNames(Json::arrayValue);
    for (LevelData ld : wd.levels)
    {
        levelNames.append(ld.name);

        Json::Value canvasPos(Json::arrayValue);
        canvasPos.append((int)ld.canvasPos.x);
        canvasPos.append((int)ld.canvasPos.y);
        data["levels"][ld.name]["canvas_pos"] = canvasPos;
        Json::Value canvasSize(Json::arrayValue);
        canvasSize.append((int)ld.canvasSize.x);
        canvasSize.append((int)ld.canvasSize.y);
        data["levels"][ld.name]["canvas_size"] = canvasSize;
        Json::Value canvasColor(Json::arrayValue);
        canvasColor.append((float)ld.canvasColor.r);
        canvasColor.append((float)ld.canvasColor.g);
        canvasColor.append((float)ld.canvasColor.b);
        data["levels"][ld.name]["canvas_color"] = canvasColor;

        Json::Value layerKeys(Json::arrayValue);
        Json::Value entityKeys(Json::arrayValue);
        Json::Value textureKeys(Json::arrayValue);
        Json::Value placedEntities(Json::arrayValue);
        Json::Value placedTextEntities(Json::arrayValue);
        Json::Value placedParticleEntities(Json::arrayValue);

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
        for (s2d::EntityData *ed : ld.entitiesData)
        {
            entityKeys.append(ed->label);
            Json::Value entData;
            entData["texture"] = ed->texture.name;
            Json::Value size(Json::arrayValue);
            size.append(ed->size.x);
            size.append(ed->size.y);
            entData["size"] = size;
            Json::Value color(Json::arrayValue);
            color.append(ed->colour.x);
            color.append(ed->colour.y);
            color.append(ed->colour.z);
            entData["color"] = color;
            data["levels"][ld.name]["entity_data"][ed->label] = entData;
        }

        // placed entities
        for (s2d::SceneEntityData *sed : ld.sceneEntityData)
        {
            Json::Value entData;
            entData["label"] = sed->label;
            entData["layer"] = sed->layer;
            entData["rotation"] = sed->rotation;
            entData["alpha"] = sed->alpha;
            Json::Value pos(Json::arrayValue);
            pos.append((int)sed->pos.x);
            pos.append((int)sed->pos.y);
            entData["pos"] = pos;
            Json::Value worldPos(Json::arrayValue);
            worldPos.append((int)sed->worldPos.x);
            worldPos.append((int)sed->worldPos.y);
            entData["worldPos"] = worldPos;
            Json::Value size(Json::arrayValue);
            size.append(sed->size.x);
            size.append(sed->size.y);
            entData["size"] = size;
            Json::Value color(Json::arrayValue);
            color.append(sed->color.r);
            color.append(sed->color.g);
            color.append(sed->color.b);
            entData["color"] = color;
            entData["texture"] = sed->texture;
            // add movement path
            Json::Value movementPath(Json::arrayValue);
            for (glm::vec2 p : sed->path)
            {
                Json::Value movementPathPos(Json::arrayValue);
                movementPathPos.append(p.x);
                movementPathPos.append(p.y);
                movementPath.append(movementPathPos);
            }
            entData["movement_path"] = movementPath;
            // add custom attributes
            placedEntities.append(entData);
        }

        // placed text entities
        for (s2d::SceneTextEntityData *ted : ld.sceneTextEntityData)
        {
            Json::Value entData;
            entData["text"] = ted->text;
            entData["layer"] = ted->layer;
            entData["alpha"] = ted->alpha;
            Json::Value pos(Json::arrayValue);
            pos.append((int)ted->pos.x);
            pos.append((int)ted->pos.y);
            entData["pos"] = pos;
            Json::Value worldPos(Json::arrayValue);
            worldPos.append((int)ted->worldPos.x);
            worldPos.append((int)ted->worldPos.y);
            entData["worldPos"] = worldPos;
            Json::Value textScale(Json::arrayValue);
            textScale.append(ted->textScale.x);
            textScale.append(ted->textScale.y);
            entData["text_scale"] = textScale;
            Json::Value color(Json::arrayValue);
            color.append(ted->color.x);
            color.append(ted->color.y);
            color.append(ted->color.z);
            entData["color"] = color;
            placedTextEntities.append(entData);
        }

        // placed particle entities
        for (s2d::SceneParticleEntityData *ped : ld.SceneParticleEntityData)
        {
            Json::Value entData;
            entData["layer"] = ped->layer;
            entData["amount"] = ped->amount;
            entData["alpha"] = ped->alpha;
            entData["direction"] = ped->direction;
            entData["dispersion"] = ped->dispersion;
            entData["scale"] = ped->scale;
            entData["velocity"] = ped->velocity;
            Json::Value pos(Json::arrayValue);
            pos.append((int)ped->pos.x);
            pos.append((int)ped->pos.y);
            entData["pos"] = pos;
            Json::Value worldPos(Json::arrayValue);
            worldPos.append((int)ped->worldPos.x);
            worldPos.append((int)ped->worldPos.y);
            entData["worldPos"] = worldPos;
            Json::Value color(Json::arrayValue);
            color.append(ped->color.x);
            color.append(ped->color.y);
            color.append(ped->color.z);
            entData["color"] = color;
            entData["texture"] = ped->texture;
            placedParticleEntities.append(entData);
        }

        data["levels"][ld.name]["layers"] = layerKeys;
        data["levels"][ld.name]["texture_keys"] = textureKeys;
        data["levels"][ld.name]["entity_keys"] = entityKeys;
        data["levels"][ld.name]["placed_entities"] = placedEntities;
        data["levels"][ld.name]["placed_text_entities"] = placedTextEntities;
        data["levels"][ld.name]["placed_particle_entities"] = placedParticleEntities;
    }
    data["level_names"] = levelNames;

    // writing connection data
    Json::Value connections(Json::arrayValue);
    for (ConnectionData cd : wd.connections)
    {
        Json::Value connection;
        connection["start"] = cd.start;
        connection["end"] = cd.end;
        connection["width"] = cd.width;
        connections.append(connection);
    }
    data["connections"] = connections;

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

WorldData Project::load()
{
    // read project from file
    std::ifstream f(path);
    Json::Value data;
    f >> data;

    WorldData wd = {};
    std::vector<LevelData> lds = {};
    std::vector<ConnectionData> cds = {};

    // iterate over levels
    const Json::Value levelNames = data["level_names"];
    for (int i = 0; i < levelNames.size(); i++)
    {
        std::string levelName = levelNames[i].asString();
        // unpack into level data struct
        LevelData ld;
        ld.name = levelName;

        // canvas
        const Json::Value canvasPosData = data["levels"][ld.name]["canvas_pos"];
        ld.canvasSize = glm::vec2(canvasPosData[0].asFloat(), canvasPosData[1].asFloat());
        const Json::Value canvasSizeData = data["levels"][ld.name]["canvas_size"];
        ld.canvasSize = glm::vec2(canvasSizeData[0].asFloat(), canvasSizeData[1].asFloat());
        const Json::Value canvasColorData = data["levels"][ld.name]["canvas_color"];
        ld.canvasColor = glm::vec3(canvasColorData[0].asFloat(), canvasColorData[1].asFloat(), canvasColorData[2].asFloat());

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
        std::vector<s2d::EntityData *> entitiesData = {};
        for (int i = 0; i < levelEntitiesData.size(); i++)
        {
            std::string entityName = levelEntitiesData[i].asString();
            Json::Value entData = data["levels"][ld.name]["entity_data"][entityName];
            std::string entityTexture = entData["texture"].asString();
            s2d::EntityData *ed = new s2d::EntityData();
            ed->label = entityName;
            ResourceManager::TextureData td = rm->getTexture(entityTexture);
            ed->texture = td;
            ed->size = glm::vec2(entData["size"][0].asFloat(), entData["size"][1].asFloat());
            ed->colour = glm::vec3(
                entData["color"][0].asFloat(),
                entData["color"][1].asFloat(),
                entData["color"][2].asFloat());
            entitiesData.push_back(ed);
        }

        // placed entities data
        const Json::Value placedEntitiesData = data["levels"][ld.name]["placed_entities"];
        std::vector<s2d::SceneEntityData *> sceneEntitiesData = {};
        for (int i = 0; i < placedEntitiesData.size(); i++)
        {
            Json::Value entData = placedEntitiesData[i];
            s2d::SceneEntityData *sceneEnt = new s2d::SceneEntityData();
            sceneEnt->label = entData["label"].asString();
            sceneEnt->layer = entData["layer"].asString();
            sceneEnt->rotation = entData["rotation"].asFloat();
            sceneEnt->alpha = entData["alpha"].asFloat();
            sceneEnt->pos = glm::vec2(entData["pos"][0].asFloat(), entData["pos"][1].asFloat());
            sceneEnt->worldPos = glm::vec2(entData["worldPos"][0].asFloat(), entData["worldPos"][1].asFloat());
            sceneEnt->size = glm::vec2(entData["size"][0].asFloat(), entData["size"][1].asFloat());
            sceneEnt->color = glm::vec3(
                entData["color"][0].asFloat(),
                entData["color"][1].asFloat(),
                entData["color"][2].asFloat());
            sceneEnt->texture = entData["texture"].asString();
            // iterate over movement path
            const Json::Value entMovementPath = entData["movement_path"];
            std::vector<glm::vec2> path = {};
            for (int j = 0; j < entMovementPath.size(); j++)
                path.push_back(glm::vec2(entMovementPath[j][0].asFloat(), entMovementPath[j][1].asFloat()));
            sceneEnt->path = path;
            sceneEntitiesData.push_back(sceneEnt);
        }

        // placed text entities data
        const Json::Value placedTextEntitiesData = data["levels"][ld.name]["placed_text_entities"];
        std::vector<s2d::SceneTextEntityData *> sceneTextEntitiesData = {};
        for (int i = 0; i < placedTextEntitiesData.size(); i++)
        {
            Json::Value entData = placedTextEntitiesData[i];
            s2d::SceneTextEntityData *sceneTextEnt = new s2d::SceneTextEntityData();
            sceneTextEnt->text = entData["text"].asString();
            sceneTextEnt->layer = entData["layer"].asString();
            sceneTextEnt->alpha = entData["alpha"].asFloat();
            sceneTextEnt->pos = glm::vec2(entData["pos"][0].asFloat(), entData["pos"][1].asFloat());
            sceneTextEnt->worldPos = glm::vec2(entData["worldPos"][0].asFloat(), entData["worldPos"][1].asFloat());
            sceneTextEnt->color = glm::vec3(
                entData["color"][0].asFloat(),
                entData["color"][1].asFloat(),
                entData["color"][2].asFloat());
            sceneTextEnt->textScale = glm::vec2(entData["text_scale"][0].asFloat(), entData["text_scale"][1].asFloat());
            sceneTextEntitiesData.push_back(sceneTextEnt);
        }

        // placed particle entities data
        const Json::Value placedParticleEntitiesData = data["levels"][ld.name]["placed_particle_entities"];
        std::vector<s2d::SceneParticleEntityData *> sceneParticleEntitiesData = {};
        for (int i = 0; i < placedParticleEntitiesData.size(); i++)
        {
            Json::Value entData = placedParticleEntitiesData[i];
            s2d::SceneParticleEntityData *sceneParticleEnt = new s2d::SceneParticleEntityData();
            sceneParticleEnt->layer = entData["layer"].asString();
            sceneParticleEnt->amount = entData["amount"].asInt();
            sceneParticleEnt->alpha = entData["alpha"].asFloat();
            sceneParticleEnt->direction = entData["direction"].asFloat();
            sceneParticleEnt->dispersion = entData["dispersion"].asFloat();
            sceneParticleEnt->scale = entData["scale"].asFloat();
            sceneParticleEnt->velocity = entData["velocity"].asFloat();
            sceneParticleEnt->pos = glm::vec2(entData["pos"][0].asFloat(), entData["pos"][1].asFloat());
            sceneParticleEnt->worldPos = glm::vec2(entData["worldPos"][0].asFloat(), entData["worldPos"][1].asFloat());
            sceneParticleEnt->color = glm::vec3(
                entData["color"][0].asFloat(),
                entData["color"][1].asFloat(),
                entData["color"][2].asFloat());
            sceneParticleEnt->texture = entData["texture"].asString();
            sceneParticleEntitiesData.push_back(sceneParticleEnt);
        }

        ld.entitiesData = entitiesData;
        ld.textures = textures;
        ld.sceneEntityData = sceneEntitiesData;
        ld.sceneTextEntityData = sceneTextEntitiesData;
        ld.SceneParticleEntityData = sceneParticleEntitiesData;

        lds.push_back(ld);
    }

    // connections
    const Json::Value connections = data["connections"];
    for (int i = 0; i < connections.size(); i++)
    {
        const Json::Value connection = connections[i];
        ConnectionData cd = {};
        cd.start = connection["start"].asString();
        cd.end = connection["end"].asString();
        cd.width = connection["width"].asFloat();
        cds.push_back(cd);
    }

    wd.nLevels = levelNames.size();
    wd.levels = lds;
    wd.nConnections = connections.size();
    wd.connections = cds;

    std::cout << data << std::endl;
    return wd;
}

void Project::undo()
{
}

void Project::redo()
{
}
