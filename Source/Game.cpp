#include <Game.h>


#include <memory>
#include <string>

#include <Assets.h>
#include <AssetManager.h>
#include <GameData.h>
#include <Service.h>


// NOTE: Gamepad name ID depends on drivers and OS
#define XBOX360_LEGACY_NAME_ID  "Xbox Controller"
#if defined(PLATFORM_RPI)
#define XBOX360_NAME_ID     "Microsoft X-Box 360 pad"
#define PS3_NAME_ID         "PLAYSTATION(R)3 Controller"
#else
#define XBOX360_NAME_ID     "Xbox 360 Controller"
#define PS3_NAME_ID         "PLAYSTATION(R)3 Controller"
#endif


using namespace pd;


u32 Game::s_ScreenWidth;
u32 Game::s_ScreenHeight;

#if (DEBUG_BUILD)
std::chrono::steady_clock::time_point Game::s_FrameStart;
std::vector<std::string> Game::s_SystemTimes;
#endif

Game::Game()
    : m_baseBath(GetApplicationDirectory()), 
      m_renderSystem(&m_entityManager), 
      m_commandSystem(&m_entityManager),
      m_graphicsAnimator(&m_entityManager),
      m_playerController(&m_entityManager),
      m_pointerAimSystem(&m_entityManager),
      m_rotateToAimSystem(&m_entityManager),
      m_cameraSystem(&m_entityManager),
      m_particleSystem(&m_entityManager),
      m_projectileSystem(&m_entityManager),
      m_physicsSystem(&m_entityManager)
{
#if (DEBUG_BUILD)
    m_renderSystem.setName("RenderSystem");
    m_commandSystem.setName("CommandSystem");
    m_graphicsAnimator.setName("GraphicsAnimator");
    m_playerController.setName("PlayerController");
    m_pointerAimSystem.setName("PointerAimSystem");
    m_rotateToAimSystem.setName("RotateToAimSystem");
    m_cameraSystem.setName("CameraSystem");
    m_particleSystem.setName("ParticleSystem");
    m_physicsSystem.setName("PhysicsSystem");
    m_projectileSystem.setName("ProjectileSystem");
#endif
}

void pd::Game::createServices()
{
    auto assets = new AssetManager();

    auto services = std::make_unique<ServiceContainer>();
    services->add(assets);

    ServiceGetter::m_serviceContainer = std::move(services);
}

void pd::Game::loadAssets()
{
    auto& assets = ServiceGetter::get<AssetManager>();

    for (const auto& subPath : getTexturePaths())
        assets.loadTexture(m_baseBath + subPath);  // TODO: Check failure

    // TODO: Check failure
    assets.loadGameData(m_baseBath + getGameDataPath());

    // TODO: Check failure
    for (const auto& subPathPrefix : getLevelDataPaths())
        assets.loadLevelData(m_baseBath + subPathPrefix);
}

//#include <box2d/box2d.h>

void pd::Game::createGameData()
{
    //b2World world({0.0f, 0.0f});

    //const f32 pixelsToMeters = 64.0f;

    //// Body definition
    //b2BodyDef bodyDef;

    ///*
    //A static body does not move under simulation and behaves as if it has infinite mass. 
    //Internally, Box2D stores zero for the mass and the inverse mass. 
    //Static bodies can be moved manually by the user. 
    //A static body has zero velocity. Static bodies do not collide with other static or kinematic bodies.
    //*/
    //bodyDef.type = b2BodyType::b2_staticBody;
    ///*
    //A kinematic body moves under simulation according to its velocity. 
    //Kinematic bodies do not respond to forces. 
    //They can be moved manually by the user, but normally a kinematic body is moved by setting its velocity. 
    //A kinematic body behaves as if it has infinite mass, however, Box2D stores zero for the mass and the inverse mass. 
    //Kinematic bodies do not collide with other kinematic or static bodies.
    //*/
    //bodyDef.type = b2BodyType::b2_kinematicBody;
    ///*
    //A dynamic body is fully simulated. They can be moved manually by the user, but normally they move according to forces. 
    //A dynamic body can collide with all body types. A dynamic body always has finite, non-zero mass. 
    //If you try to set the mass of a dynamic body to zero, it will automatically acquire a mass of one kilogram and it won't rotate.
    //*/
    //bodyDef.type = b2BodyType::b2_dynamicBody;
    //// In meters
    //bodyDef.position.Set(0.0f, 10.0f);

    //// Shape definition
    //b2CircleShape shape;
    //shape.m_radius = 0.25f;

    //// Fixture definition
    //b2FixtureDef fixtureDef;
    //fixtureDef.shape = &shape;
    ///*
    //The fixture density is used to compute the mass properties of the parent body. 
    //The density can be zero or positive. 
    //You should generally use similar densities for all your fixtures. This will improve stacking stability.
    //*/
    //fixtureDef.density = 2.5f;    // (kg/m^2)
    //fixtureDef.friction = 0.25f;  // [0.0, 1.0]
    ///*
    //Restitution is used to make objects bounce. The restitution value is usually set to be between 0 and 1. 
    //Consider dropping a ball on a table. A value of zero means the ball won't bounce. 
    //This is called an inelastic collision. A value of one means the ball's velocity will be exactly reflected. 
    //This is called a perfectly elastic collision. Restitution is combined using the following formula.
    //*/
    //fixtureDef.restitution = 0.75f;

    //b2Body* body = world.CreateBody(&bodyDef);
    //body->CreateFixture(&fixtureDef);

    ///////////////
    //// GROUND
    //bodyDef.type = b2BodyType::b2_staticBody;
    //bodyDef.position.Set(0.0f, 0.0f);

    //// Poly line defined by vertices
    //b2ChainShape shape2;
    //std::vector<b2Vec2> chain = {
    //    {-500.0f, 0.0f},
    //    {500.0f, 0.0f},
    //};
    //shape2.CreateLoop(chain.data(), chain.size());

    //fixtureDef.shape = &shape2;
    //fixtureDef.friction = 0.5f;
    //fixtureDef.restitution = 0.0f;

    //b2Body* body2 = world.CreateBody(&bodyDef);
    //body2->CreateFixture(&fixtureDef);

    ///////////////
    //// BOX
    //bodyDef.type = b2BodyType::b2_staticBody;
    //bodyDef.position.Set(0.0f, 0.0f);

    //b2PolygonShape shape3;
    //shape3.SetAsBox(5.0f, 5.0f);

    //fixtureDef.shape = &shape3;
    //fixtureDef.friction = 0.5f;
    //fixtureDef.restitution = 0.0f;

    //b2Body* body3 = world.CreateBody(&bodyDef);
    //body3->CreateFixture(&fixtureDef);

    //body3->ApplyForceToCenter({ 1.0f, 0.0f }, true);

    ////b2Body* groundBody = world.CreateBody(&groundBodyDef);

    ////b2PolygonShape groundBox;
    ////groundBox.SetAsBox(50.0f, 10.0f);

    ////groundBody->CreateFixture(&groundBox, 0.0f);


    ////b2BodyDef bodyDef;
    ////bodyDef.type = b2_dynamicBody;
    ////bodyDef.position.Set(0.0f, 4.0f);
    ////b2Body* body = world.CreateBody(&bodyDef);

    ////b2PolygonShape dynamicBox;
    ////dynamicBox.SetAsBox(1.0f, 1.0f);

    ////b2FixtureDef fixtureDef;
    ////fixtureDef.shape = &dynamicBox;
    ////fixtureDef.density = 1.0f;
    ////fixtureDef.friction = 0.3f;

    ////body->CreateFixture(&fixtureDef);


    ////f32 timeStep = 1.0f / 60.0f;

    ////int32 velocityIterations = 8;
    ////int32 positionIterations = 3;

    ////for (int32 i = 0; i < 60; ++i)
    ////{
    ////    world.Step(timeStep, velocityIterations, positionIterations);
    ////    b2Vec2 position = body->GetPosition();
    ////    float angle = body->GetAngle();
    ////    printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
    ////}


    auto& assets = ServiceGetter::get<AssetManager>();

    //////
    // Level creation
    //
    // TODO: Move this into XML file, because we can just make the 'levelName' and 'textureName' an argument
    const std::string levelName = "test";
    auto level = assets.getLevelData(levelName);
    auto texture = assets.getTexture("sh_tiles_01");
    ASSERT_MSG(level, "No level data of given name found!");
    ASSERT_MSG(texture, "No texture data of given name found!");
    auto& levelEntity = *m_entityManager.createEntity("Level_" + levelName);
    //TileMapArgs tileMapArgs;
    //tileMapArgs.levelName = levelName;
    //tileMapArgs.textureName = "sh_tiles_01";
    //levelEntity.addComponent<TileMap>(&tileMapArgs);
    for (u32 i = 0; i < level->levelLayers.size(); ++i)
    {
        const auto& layer = level->levelLayers[i];

        for (u32 row = 0; row < layer.data.size(); ++row)
        {
            for (u32 col = 0; col < layer.data[row].size(); ++col)
            {
                const auto& textureNdx = layer.data[row][col];
                if (textureNdx == -1)
                    continue;

                auto& tile = *m_entityManager.createEntity("Level_" + levelName + "-" + std::to_string(i) + "-" + std::to_string(row) + "_" + std::to_string(col));
                
                TransformArgs tArgs;
                tArgs.position = { (f32)col * 64.0f, (f32)row * 64.0f };
                //tArgs.scale = { 1.1f, 1.1f };
                tile.addComponent<Transform>(&tArgs);
                
                GraphicsArgs gArgs;
                gArgs.textureName = "sh_tiles_01";
                gArgs.size = { 64.0f, 64.0f };
                gArgs.offset = {
                    (f32)(textureNdx % (s32)(texture->width / 64)) * 64.0f,
                    (f32)(textureNdx / (s32)(texture->height / 64)) * 64.0f,
                };
                gArgs.pivot = { 0.5f, 0.5f };
                gArgs.drawLayer = layer.drawLayer;
                tile.addComponent<Graphics>(&gArgs);

                if (i > 0)
                {
                    BoxPhysicsComponentArgs args;
                    args.bodyType = eBodyType::Static;
                    args.colliderType = eColliderType::Box;
                    args.density = 1.0f;
                    args.friction = 0.2f;
                    args.restitution = 0.0f;
                    args.width = 4.0f;
                    args.height = 4.0f;
                    tile.addComponent<StaticPhysicsComponent>(&args);
                }

                tile.postInit();
                tile.setParent(&levelEntity);
            }
        }
        // TileMap component
        // Texture pointer
        // One TileMap per layer???
    }
    //level->collisionLayer;

    //////
    // Non-Entity Singleton components
    //
    auto& singleton = *m_entityManager.createEntity("SingletonComponents");
    singleton.addComponent<Input>();
    CameraArgs cam;
    cam.screenWidth = Game::s_ScreenWidth;
    cam.screenHeight = Game::s_ScreenHeight;
    cam.position = { 2560.0, 2496.0 };
    singleton.addComponent<Camera>(&cam);
    CameraFollowArgs camFArgs;
    singleton.addComponent<CameraFollow>(&cam);
    PhysicsWorldArgs physicsArgs;
    singleton.addComponent<PhysicsWorld>(&physicsArgs);

    //////
    // Entities
    //
    std::unordered_map<size_t, Entity*> entities;

    // Components that need to be referenced by other components during intialization
    AnimationFSM* animationFSM = nullptr;

    for (const auto& entityData : assets.getGameData())
    {
        auto& entity = *m_entityManager.createEntity(entityData.name);

        for (const auto& componentData : entityData.components)
        {
            switch (componentData.type)
            {
            case eComponentType::Graphics:
            {
                GraphicsArgs args;
                loadGraphicsArgs(componentData.argsRoot, args);
                entity.addComponent<Graphics>(&args);
                break;
            }
            case eComponentType::AnimationFSM:
            {
                AnimationFSMArgs args;
                loadAnimationFSMArgs(componentData.argsRoot, args);
                entity.addComponent<AnimationFSM>(&args);
                animationFSM = entity.getComponent<AnimationFSM>();
                break;
            }
            case eComponentType::AnimationState:
            {
                ASSERT_MSG(animationFSM, "Tried adding AnimationState to entity without a parent AnimationFSM.");

                AnimationStateArgs args;
                loadAnimationStateArgs(componentData.argsRoot, args);
                entity.addComponent<AnimationState>(&args);
                AnimationState* state = entity.getComponent<AnimationState>();
                animationFSM->addState(state);
                break;
            }
            case eComponentType::Transform:
            {
                TransformArgs args;
                loadTransformArgs(componentData.argsRoot, args);
                entity.addComponent<Transform>(&args);
                break;
            }
            case eComponentType::ActorCommand:
            {
                ActorCommandArgs args;
                loadActorCommandArgs(componentData.argsRoot, args);
                entity.addComponent<ActorCommand>(&args);
                break;
            }
            case eComponentType::PointerAim:
            {
                PointerAimArgs args;
                entity.addComponent<PointerAimComponent>(&args);
                break;
            }
            case eComponentType::RotateToAim:
            {
                RotateToAimArgs args;
                entity.addComponent<RotateToAimComponent>(&args);
                break;
            }
            case eComponentType::CameraFollow:
            {
                CameraFollowArgs args;
                entity.addComponent<CameraFollow>(&args);
                break;
            }
            case eComponentType::ParticleSystem:
            {
                ParticleSystemArgs args;
                loadParticleSystemArgs(componentData.argsRoot, args);
                entity.addComponent<ParticleSystemComponent>(&args);
                break;
            }
            case eComponentType::ActorIsMoving:
            {
                ActorIsMovingArgs args;
                entity.addComponent<ActorIsMoving>(&args);
                break;
            }
            case eComponentType::ActorIsShooting:
            {
                ActorIsShootingArgs args;
                entity.addComponent<ActorIsShooting>(&args);
                break;
            }
            case eComponentType::ProjectileSpawner:
            {
                ProjectileSpawnerArgs args;
                loadProjectileSpawnerArgs(componentData.argsRoot, args);
                entity.addComponent<ProjectileSpawner>(&args);
                break;
            }
            case eComponentType::PhysicsComponent:
            {
                if (auto arg = getComponentArg("colliderType", componentData.argsRoot.children))
                {
                    auto colliderType = parseColliderTypeArg(arg->valueAttribute.as_string());

                    switch (colliderType)
                    {
                    case eColliderType::Box:
                    {
                        BoxPhysicsComponentArgs args;
                        loadBoxPhysicsComponentArgs(componentData.argsRoot, args);
                        switch (args.bodyType)
                        {
                        case eBodyType::Static:
                        {
                            entity.addComponent<StaticPhysicsComponent>(&args);
                            break;
                        }
                        case eBodyType::Kinematic:
                        {
                            entity.addComponent<KinematicPhysicsComponent>(&args);
                            break;
                        }
                        case eBodyType::Dynamic:
                        {
                            entity.addComponent<DynamicPhysicsComponent>(&args);
                            break;
                        }
                        default:
                            break;
                        }
                    }
                    case eColliderType::Circle:
                    {
                        break;
                    }
                    case eColliderType::Polygon:
                    {
                        break;
                    }
                    case eColliderType::Edge:
                    {
                        break;
                    }
                    case eColliderType::Chain:
                    {
                        break;
                    }
                    default:
                        ASSERT_MSG(false, "Illegal eColliderType passed.");
                    }
                }
                else
                    ASSERT_MSG(false, "Required argument colliderType not found!");
                break;
            }
            //case eComponentType::PlayerCommand:
            //case eComponentType::FilterBits:
            case eComponentType::Input:
                break;
            case eComponentType::Null:
                break;
            default:
                ASSERT_MSG(false, "Unknown component type passed!");
            }
        }

        // Set entity parent
        if (entityData.parent.size() > 0)
        {
            auto key = std::hash<std::string_view>{}(entityData.parent);
            auto it = entities.find(key);
            ASSERT_MSG(it != entities.end(), "Unable to find entity of name `%s`.", entityData.parent.c_str());
            entity.setParent(it->second);
        }

        auto key = std::hash<std::string_view>{}(entityData.name);
        entities.emplace(key, &entity);
    }

    for (auto& [_, pEntity] : entities)
        pEntity->postInit();

    //auto& player = *m_entityManager.createEntity("Player");

    //GraphicsArgs graphicsArgs;
    //graphicsArgs.textureName = "body";
    //graphicsArgs.rotation = 90.0f;
    //player.addComponent<Graphics>(&graphicsArgs);
    //TransformArgs transArgs;
    //transArgs.position = { 1920.0f * 0.35f, 1080.0f * 0.5f };
    //transArgs.scale = { 0.75f, 0.75f };
    //player.addComponent<Transform>(&transArgs);
    //PlayerCommandArgs cmdArgs;
    //cmdArgs.isLocalPlayer = true;
    //player.addComponent<PlayerCommand>(&cmdArgs);

    //auto& playerGun = *m_entityManager.createEntity("PlayerGun");
    //GraphicsArgs graphicsArgs2;
    //graphicsArgs2.textureName = "gun";
    //graphicsArgs2.rotation = 90.0f;
    //graphicsArgs2.drawLayer = 1;
    //playerGun.addComponent<Graphics>(&graphicsArgs2);
    //TransformArgs transArgs2;
    //playerGun.addComponent<Transform>(&transArgs2);
    //playerGun.setParent(&player);

    //auto& playerTracksLeft = *m_entityManager.createEntity("PlayerTracksLeft");
    //GraphicsArgs graphicsArgs3;
    //graphicsArgs3.textureName = "tracks_left";
    //graphicsArgs3.rotation = 90.0f;
    //graphicsArgs3.drawLayer = -1;
    //playerTracksLeft.addComponent<Graphics>(&graphicsArgs3);
    //TransformArgs transArgs3;
    //playerTracksLeft.addComponent<Transform>(&transArgs3);
    //playerTracksLeft.setParent(&player);

    //auto& playerTracksRight = *m_entityManager.createEntity("PlayerTracksRight");
    //GraphicsArgs graphicsArgs4;
    //graphicsArgs4.textureName = "tracks_right";
    //graphicsArgs4.rotation = 90.0f;
    //graphicsArgs4.drawLayer = -1;
    //playerTracksRight.addComponent<Graphics>(&graphicsArgs4);
    //TransformArgs transArgs4;
    //playerTracksRight.addComponent<Transform>(&transArgs4);
    //playerTracksRight.setParent(&player);
}

s32 Game::run()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    SetConfigFlags(FLAG_MSAA_4X_HINT);  // Set MSAA 4X hint before windows creation

    InitWindow(screenWidth, screenHeight, "PRJ005 - Untitled Tank Game");

    // TODO: I would like to get the actual display width/height
    s_ScreenWidth = GetScreenWidth();
    s_ScreenHeight = GetScreenHeight();

    //SetExitKey(KEY_NULL);       // Disable KEY_ESCAPE to close window, X-button still works

    createServices();
    loadAssets();
    createGameData();

    m_entityManager.registerSystem(&m_commandSystem);
    m_entityManager.registerSystem(&m_playerController);
    m_entityManager.registerSystem(&m_physicsSystem);
    m_entityManager.registerSystem(&m_cameraSystem);
    m_entityManager.registerSystem(&m_pointerAimSystem);
    m_entityManager.registerSystem(&m_rotateToAimSystem);
    m_entityManager.registerSystem(&m_projectileSystem);
    m_entityManager.registerSystem(&m_graphicsAnimator);
    m_entityManager.registerSystem(&m_particleSystem);
    m_entityManager.registerSystem(&m_renderSystem);

    m_entityManager.initialize();

    /*
    Command : Component
    ActorCommand : Command
        moveX
        moveY
        shoot
    PlayerCommand : ActorCommand
        openMenu
    */

    // TODO: Draw a sprite on the screen
    // TODO: AssetManager

    // Camera [Component]
    // Command
    // MoveCommand : Command
    // ShootCommand : Command
    // ...
    // Input [Component]
    //# Movement [Component] #
    // Collider [Component]
    // Graphics [Component]
    // AI [Component]
    // Health [Component]
    // ParticleSystem
    // ProjectileEmitter [ParticleSystem]
    // ProjectileEffect [Component]
    // ExplosionEmitter [ParticleSystem]
    // ExplosionEffect [Component]

#pragma region DELETE
    //Vector2 ballPosition = { (float)screenWidth / 2, (float)screenHeight / 2 };
    //Color ballColor = DARKBLUE;
    //int boxPositionY = screenHeight / 2 - 40;
    //int scrollSpeed = 4;            // Scrolling speed in pixels
    //int gamepad = 0; // which gamepad to display
#pragma endregion
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
#if (DEBUG_BUILD)
        s_SystemTimes.clear();
        s_FrameStart = std::chrono::high_resolution_clock::now();
#endif
        m_entityManager.update();
#pragma region DELETE
        // Update
        //----------------------------------------------------------------------------------
        //if (IsKeyDown(KEY_RIGHT)) ballPosition.x += 2.0f;
        //if (IsKeyDown(KEY_LEFT)) ballPosition.x -= 2.0f;
        //if (IsKeyDown(KEY_UP)) ballPosition.y -= 2.0f;
        //if (IsKeyDown(KEY_DOWN)) ballPosition.y += 2.0f;

        //if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) ballColor = MAROON;
        //else if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) ballColor = LIME;
        //else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) ballColor = DARKBLUE;
        //else if (IsMouseButtonPressed(MOUSE_BUTTON_SIDE)) ballColor = PURPLE;
        //else if (IsMouseButtonPressed(MOUSE_BUTTON_EXTRA)) ballColor = YELLOW;
        //else if (IsMouseButtonPressed(MOUSE_BUTTON_FORWARD)) ballColor = ORANGE;
        //else if (IsMouseButtonPressed(MOUSE_BUTTON_BACK)) ballColor = BEIGE;

        //boxPositionY -= ((int)GetMouseWheelMove() * scrollSpeed);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        //BeginDrawing();

        //ClearBackground(BLACK);

        //if (IsKeyPressed(KEY_LEFT) && gamepad > 0) gamepad--;
        //if (IsKeyPressed(KEY_RIGHT)) gamepad++;

        //if (IsGamepadAvailable(gamepad))
        //{
        //    DrawText(TextFormat("GP%d: %s", gamepad, GetGamepadName(gamepad)), 10, 10, 10, BLACK);

        //    if (true)
        //    {
        //        DrawTexture(texXboxPad, 0, 0, DARKGRAY);

        //        // Draw buttons: xbox home
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE)) DrawCircle(394, 89, 19, RED);

        //        // Draw buttons: basic
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE_RIGHT)) DrawCircle(436, 150, 9, RED);
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE_LEFT)) DrawCircle(352, 150, 9, RED);
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) DrawCircle(501, 151, 15, BLUE);
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) DrawCircle(536, 187, 15, LIME);
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) DrawCircle(572, 151, 15, MAROON);
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_UP)) DrawCircle(536, 115, 15, GOLD);

        //        // Draw buttons: d-pad
        //        DrawRectangle(317, 202, 19, 71, BLACK);
        //        DrawRectangle(293, 228, 69, 19, BLACK);
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_UP)) DrawRectangle(317, 202, 19, 26, RED);
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) DrawRectangle(317, 202 + 45, 19, 26, RED);
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) DrawRectangle(292, 228, 25, 19, RED);
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) DrawRectangle(292 + 44, 228, 26, 19, RED);

        //        // Draw buttons: left-right back
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_TRIGGER_1)) DrawCircle(259, 61, 20, RED);
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_1)) DrawCircle(536, 61, 20, RED);

        //        // Draw axis: left joystick

        //        Color leftGamepadColor = BLACK;
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_THUMB)) leftGamepadColor = RED;
        //        DrawCircle(259, 152, 39, BLACK);
        //        DrawCircle(259, 152, 34, LIGHTGRAY);
        //        DrawCircle(259 + (int)(GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X) * 20),
        //            152 + (int)(GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_Y) * 20), 25, leftGamepadColor);

        //        // Draw axis: right joystick
        //        Color rightGamepadColor = BLACK;
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_THUMB)) rightGamepadColor = RED;
        //        DrawCircle(461, 237, 38, BLACK);
        //        DrawCircle(461, 237, 33, LIGHTGRAY);
        //        DrawCircle(461 + (int)(GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_X) * 20),
        //            237 + (int)(GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_Y) * 20), 25, rightGamepadColor);

        //        // Draw axis: left-right triggers
        //        DrawRectangle(170, 30, 15, 70, GRAY);
        //        DrawRectangle(604, 30, 15, 70, GRAY);
        //        DrawRectangle(170, 30, 15, (int)(((1 + GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_TRIGGER)) / 2) * 70), RED);
        //        DrawRectangle(604, 30, 15, (int)(((1 + GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_TRIGGER)) / 2) * 70), RED);

        //        //DrawText(TextFormat("Xbox axis LT: %02.02f", GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_TRIGGER)), 10, 40, 10, BLACK);
        //        //DrawText(TextFormat("Xbox axis RT: %02.02f", GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_TRIGGER)), 10, 60, 10, BLACK);
        //    }
        //    else if (TextIsEqual(GetGamepadName(gamepad), PS3_NAME_ID))
        //    {
        //        DrawTexture(texPs3Pad, 0, 0, DARKGRAY);

        //        // Draw buttons: ps
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE)) DrawCircle(396, 222, 13, RED);

        //        // Draw buttons: basic
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE_LEFT)) DrawRectangle(328, 170, 32, 13, RED);
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_MIDDLE_RIGHT)) DrawTriangle(Vector2{ 436, 168 }, Vector2{ 436, 185 }, Vector2{ 464, 177 }, RED);
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_UP)) DrawCircle(557, 144, 13, LIME);
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) DrawCircle(586, 173, 13, RED);
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) DrawCircle(557, 203, 13, VIOLET);
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) DrawCircle(527, 173, 13, PINK);

        //        // Draw buttons: d-pad
        //        DrawRectangle(225, 132, 24, 84, BLACK);
        //        DrawRectangle(195, 161, 84, 25, BLACK);
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_UP)) DrawRectangle(225, 132, 24, 29, RED);
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) DrawRectangle(225, 132 + 54, 24, 30, RED);
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) DrawRectangle(195, 161, 30, 25, RED);
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) DrawRectangle(195 + 54, 161, 30, 25, RED);

        //        // Draw buttons: left-right back buttons
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_TRIGGER_1)) DrawCircle(239, 82, 20, RED);
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_1)) DrawCircle(557, 82, 20, RED);

        //        // Draw axis: left joystick
        //        Color leftGamepadColor = BLACK;
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_THUMB)) leftGamepadColor = RED;
        //        DrawCircle(319, 255, 35, leftGamepadColor);
        //        DrawCircle(319, 255, 31, LIGHTGRAY);
        //        DrawCircle(319 + (int)(GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X) * 20),
        //            255 + (int)(GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_Y) * 20), 25, leftGamepadColor);

        //        // Draw axis: right joystick
        //        Color rightGamepadColor = BLACK;
        //        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_THUMB)) rightGamepadColor = RED;
        //        DrawCircle(475, 255, 35, BLACK);
        //        DrawCircle(475, 255, 31, LIGHTGRAY);
        //        DrawCircle(475 + (int)(GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_X) * 20),
        //            255 + (int)(GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_Y) * 20), 25, rightGamepadColor);

        //        // Draw axis: left-right triggers
        //        DrawRectangle(169, 48, 15, 70, GRAY);
        //        DrawRectangle(611, 48, 15, 70, GRAY);
        //        DrawRectangle(169, 48, 15, (int)(((1 - GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_TRIGGER)) / 2) * 70), RED);
        //        DrawRectangle(611, 48, 15, (int)(((1 - GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_TRIGGER)) / 2) * 70), RED);
        //    }
        //    else
        //    {
        //        DrawText("- GENERIC GAMEPAD -", 280, 180, 20, GRAY);

        //        // TODO: Draw generic gamepad
        //    }

        //    DrawText(TextFormat("DETECTED AXIS [%i]:", GetGamepadAxisCount(0)), 10, 50, 10, MAROON);

        //    for (int i = 0; i < GetGamepadAxisCount(0); i++)
        //    {
        //        DrawText(TextFormat("AXIS %i: %.02f", i, GetGamepadAxisMovement(0, i)), 20, 70 + 20 * i, 10, DARKGRAY);
        //    }

        //    if (GetGamepadButtonPressed() != GAMEPAD_BUTTON_UNKNOWN) DrawText(TextFormat("DETECTED BUTTON: %i", GetGamepadButtonPressed()), 10, 430, 10, RED);
        //    else DrawText("DETECTED BUTTON: NONE", 10, 430, 10, GRAY);
        //}
        //else
        //{
        //    DrawText(TextFormat("GP%d: NOT DETECTED", gamepad), 10, 10, 10, GRAY);

        //    DrawTexture(texXboxPad, 0, 0, LIGHTGRAY);
        //}


        //DrawText("move the ball with arrow keys", 10, 10, 20, DARKGRAY);

        //DrawCircleV(ballPosition, 40, ballColor);
        //DrawRectangle(screenWidth / 2 - 40, boxPositionY, 80, 80, MAROON);

        //EndDrawing();
        //----------------------------------------------------------------------------------
#pragma endregion
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
