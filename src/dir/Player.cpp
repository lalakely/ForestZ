#include "../include/Player.hpp"
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreMaterialManager.h>
#include <iostream>

Player::Player() : playerBody(nullptr), playerNode(nullptr), playerEntity(nullptr),
    playerAnimation(nullptr), currentAnimation(nullptr), health(100.0f), maxHealth(100.0f),
    energy(100.0f), maxEnergy(100.0f), energyRegenRate(10.0f), lastDamageTime(0.0f),
    healthRegenDelay(0.0f) {
}

Player::~Player() {
    // Clean up physics resources
    if (playerBody) {
        if (playerBody->getMotionState()) {
            delete playerBody->getMotionState();
        }
        if (playerBody->getCollisionShape()) {
            delete playerBody->getCollisionShape();
        }
        delete playerBody;
        playerBody = nullptr;
    }

    // Clean up Ogre resources
    if (playerEntity && playerNode) {
        playerNode->detachObject(playerEntity);
        playerNode->getCreator()->destroyEntity(playerEntity);
        playerEntity = nullptr;
    }
    if (playerNode) {
        playerNode->getCreator()->destroySceneNode(playerNode);
        playerNode = nullptr;
    }
}

void Player::createCamera(SceneManager* scnMgr , Camera * cam) {
    if (!scnMgr) {
        std::cerr << "Error: SceneManager is null in createCamera" << std::endl;
        return;
    }

    // Create a camera node
    playerNode = scnMgr->getRootSceneNode()->createChildSceneNode("PlayerCameraNode");
    if (!playerNode) {
        std::cerr << "Error: Failed to create player camera node" << std::endl;
        return;
    }

    // Create a camera and attach it to the node
    cam = scnMgr->createCamera("PlayerCamera");
    if (!cam) {
        std::cerr << "Error: Failed to create player camera" << std::endl;
        return;
    }
    
    cam->setNearClipDistance(5);
    playerNode->attachObject(cam);
    
    // Set initial camera position
    playerNode->setPosition(Vector3(0, 50, 100)); // Position above the player

}

void Player::createPlayer(SceneManager* scnMgr, Vector3 position, btDiscreteDynamicsWorld* dynamicsWorld) {
    if (!scnMgr || !dynamicsWorld) {
        std::cerr << "Error: SceneManager or DynamicsWorld is null in createPlayer" << std::endl;
        return;
    }

    // Create a new entity for the player
    try {
        playerEntity = scnMgr->createEntity("Player", "Mesh.mesh");
        
        // List all animations available in the mesh
        std::cout << "\n=== Available Animations in Mesh.mesh ===\n";
        Ogre::AnimationStateSet* animationStates = playerEntity->getAllAnimationStates();
        if (animationStates) {
            Ogre::AnimationStateIterator animationIterator = animationStates->getAnimationStateIterator();
            while (animationIterator.hasMoreElements()) {
                Ogre::AnimationState* anim = animationIterator.getNext();
                std::cout << "Animation name: " << anim->getAnimationName() 
                         << ", Length: " << anim->getLength() << " seconds" << std::endl;
            }
        } else {
            std::cout << "No animations found in the mesh.\n";
        }
        std::cout << "===================================\n\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error creating player entity: " << e.what() << std::endl;
        return;
    }

    // Create a new scene node for the player
    playerNode = scnMgr->getRootSceneNode()->createChildSceneNode(position);
    if (!playerNode) {
        std::cerr << "Error: Failed to create player scene node" << std::endl;
        scnMgr->destroyEntity(playerEntity);
        playerEntity = nullptr;
        return;
    }

    // Attach the entity to the scene node
    playerNode->attachObject(playerEntity);

    // Set the scale and position of the player
    playerNode->setScale(0.5f, 0.5f, 0.5f);
    playerNode->setPosition(position);
    playerEntity->setCastShadows(true);

    // Set initial orientation to face NEGATIVE_UNIT_Z
    playerNode->setOrientation(Quaternion(Degree(90), Vector3::UNIT_Y));

    // Apply material
    playerEntity->setMaterialName("Ch15_body1.001");

    // Créer et attacher le pistolet
    try {
        Entity* gunEntity = scnMgr->createEntity("PlayerGun", "Cylinder.mesh");
        if (!gunEntity) {
            std::cerr << "Failed to create gun entity" << std::endl;
            return;
        }

        // Créer un nœud enfant pour le pistolet
        SceneNode* gunNode = playerNode->createChildSceneNode("GunNode");
        if (!gunNode) {
            std::cerr << "Failed to create gun node" << std::endl;
            scnMgr->destroyEntity(gunEntity);
            return;
        }

        Light * gunLight = scnMgr->createLight("GunLight");
        gunLight->setType(Light::LT_POINT);
        gunLight->setDiffuseColour(1.0f, 1.0f, 1.0f);  // Orange
        gunLight->setSpecularColour(1.0f, 1.0f, 1.0f);  // Orange
        gunLight->setPowerScale(1.0f);  // Augmenter la puissance de la lumière
        SceneNode* gunLightNode = gunNode->createChildSceneNode("GunLightNode");
        gunLightNode->attachObject(gunLight);

        // Attacher le pistolet au nœud
        gunNode->attachObject(gunEntity);
        
        // Ajuster la position et l'échelle du pistolet pour qu'il soit plus visible
        gunNode->setPosition(-20, 125, 25);  // Position à droite, à hauteur d'épaule et DEVANT le joueur (Z négatif)
        gunNode->setScale(1.0f,1.0f, 1.0f);  // Beaucoup plus grand dans toutes les dimensions
        
        // Orienter le pistolet avec une rotation supplémentaire sur l'axe Z
        gunNode->setOrientation(Quaternion(Degree(0), Vector3::UNIT_Y) * 
                              Quaternion(Degree(0), Vector3::UNIT_X) *
                              Quaternion(Degree(180), Vector3::UNIT_Z));

        // Appliquer un matériau métallique au pistolet
        MaterialPtr gunMaterial = MaterialManager::getSingleton().create(
            "Material.002", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        Pass* pass = gunMaterial->getTechnique(0)->getPass(0);
        pass->setDiffuse(0.7f, 0.7f, 0.7f, 1.0f);  // Gris métallique
        pass->setSpecular(1.0f, 1.0f, 1.0f, 1.0f);  // Reflet brillant
        pass->setShininess(80.0f);  // Surface très brillante
        gunEntity->setMaterialName("Material.002");

        std::cout << "Gun successfully attached to player" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error creating gun entity: " << e.what() << std::endl;
    }

    // Create a collision shape for the player
    btCollisionShape* playerShape = new btCapsuleShape(5.0f, 35.0f);

    // Set the player's initial position in the physics world
    btTransform playerTransform;
    playerTransform.setIdentity();
    playerTransform.setOrigin(btVector3(position.x, position.y + 20.0f, position.z));

    // Create a rigid body for the player
    btScalar mass = 150.0f;
    btVector3 localInertia(0, 0, 0);
    playerShape->calculateLocalInertia(mass, localInertia);
    btDefaultMotionState* motionState = new btDefaultMotionState(playerTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, playerShape, localInertia);
    
    // Ajuster les paramètres de construction avant la création du corps rigide
    rbInfo.m_friction = 15.0f;
    rbInfo.m_rollingFriction = 15.0f;
    rbInfo.m_spinningFriction = 15.0f;
    rbInfo.m_restitution = 0.0f;
    rbInfo.m_linearDamping = 0.95f;
    rbInfo.m_angularDamping = 0.95f;

    playerBody = new btRigidBody(rbInfo);

    // Prevent rotation on X and Z axes to keep player upright
    playerBody->setAngularFactor(btVector3(0, 1, 0));

    // Adjust additional friction parameters
    playerBody->setFriction(15.0f);
    playerBody->setRollingFriction(15.0f);
    playerBody->setSpinningFriction(15.0f);
    playerBody->setRestitution(0.0f);
    playerBody->setDamping(0.95f, 0.95f);

    // Configure le corps rigide pour un meilleur contrôle
    playerBody->setActivationState(DISABLE_DEACTIVATION);
    playerBody->setSleepingThresholds(0.0f, 0.0f);

    // Contraintes de mouvement supplémentaires
    playerBody->setLinearFactor(btVector3(1, 0, 1));  // Bloque le mouvement vertical

    // Add the rigid body to the physics world
    dynamicsWorld->addRigidBody(playerBody);

    // Définir les flags de collision pour améliorer la détection de contact
    playerBody->setCollisionFlags(playerBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

    // Try to get the default animation
    try {
        if (playerEntity->hasAnimationState("walkForward")) {
            playerAnimation = playerEntity->getAnimationState("walkForward");
            if (playerAnimation) {
                playerAnimation->setEnabled(false);  // Désactivé par défaut, sera activé lors du mouvement
                playerAnimation->setLoop(true);
                std::cout << "Animation successfully loaded and ready\n";
            }
        } else {
            std::cout << "Warning: 'walkForward' animation not found.\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error setting up animation: " << e.what() << std::endl;
    }

    // Initialize with Idle animation
    if (playerEntity->hasAnimationState("Idle")) {
        setAnimation("Idle");
    }
}

void Player::shoot(SceneManager* scnMgr, btDiscreteDynamicsWorld* dynamicsWorld, Vector3 direction) {
    // Créer un nom unique pour la balle
    static unsigned long bulletCounter = 0;
    String uniqueName = "Bullet_" + std::to_string(++bulletCounter) + "_" + 
                       std::to_string(std::rand()) + "_" +
                       std::to_string(Ogre::Root::getSingleton().getTimer()->getMilliseconds());

    // Créer une balle en utilisant une primitive de sphère
    Entity* bulletEntity = scnMgr->createEntity(uniqueName, SceneManager::PT_SPHERE);
    SceneNode* bulletNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    bulletNode->attachObject(bulletEntity);
    bulletNode->setScale(0.05f, 0.05f, 0.05f);  // Réduit de moitié (était 0.1f)
    
    // Créer un matériau personnalisé pour la balle
    MaterialPtr bulletMaterial = MaterialManager::getSingleton().create(
        uniqueName + "_material", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
    );
    Pass* pass = bulletMaterial->getTechnique(0)->getPass(0);
    pass->setDiffuse(1.0f, 0.0f, 0.0f, 1.0f);  // Rouge
    pass->setAmbient(0.5f, 0.0f, 0.0f);        // Rouge sombre
    pass->setSpecular(1.0f, 1.0f, 1.0f, 1.0f); // Reflet blanc
    pass->setShininess(32.0f);
    
    // Activer la profondeur
    pass->setDepthCheckEnabled(true);
    pass->setDepthWriteEnabled(true);
    
    // Appliquer le matériau à la balle
    bulletEntity->setMaterialName(uniqueName + "_material");
    
    // Position initiale à hauteur de la tête (environ 50 unités au-dessus de la position du joueur)
    Vector3 startPosition = playerNode->getPosition() + Vector3(0, 50, 0);
    
    // Utiliser exactement la direction du pointeur
    Vector3 shootDirection = direction;
    // Normaliser la direction pour avoir un vecteur unitaire
    shootDirection.normalise();
    
    // Ajouter un offset dans la direction du tir pour éviter la collision avec le joueur
    startPosition += shootDirection * 5;  // Augmenté à 5 pour s'assurer que la balle part devant le joueur
    bulletNode->setPosition(startPosition);
    
    // Configuration pour une balle réelle
    btCollisionShape* sphereShape = new btSphereShape(COLLISION_RADIUS);
    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin(btVector3(startPosition.x, startPosition.y, startPosition.z));
    
    btScalar mass = BULLET_MASS;
    btVector3 localInertia(0, 0, 0);
    sphereShape->calculateLocalInertia(mass, localInertia);
    
    btDefaultMotionState* motionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, sphereShape, localInertia);
    
    // Paramètres pour une balle réelle
    rbInfo.m_restitution = 0.0f;        // Pas de rebond
    rbInfo.m_friction = 0.0f;           // Pas de friction
    rbInfo.m_rollingFriction = 0.0f;    // Pas de friction de roulement
    rbInfo.m_linearDamping = 0.0f;      // Pas d'amortissement
    rbInfo.m_angularDamping = 0.0f;     // Pas d'amortissement de rotation
    
    btRigidBody* bulletBody = new btRigidBody(rbInfo);
    
    // Pas de gravité pour les balles
    bulletBody->setGravity(btVector3(0, 0, 0));
    
    // Activer la balle et ses collisions
    bulletBody->setActivationState(DISABLE_DEACTIVATION);
    
    // Vélocité initiale très élevée
    btVector3 velocity(shootDirection.x * BULLET_SPEED,
                      shootDirection.y * BULLET_SPEED,
                      shootDirection.z * BULLET_SPEED);
    
    // Appliquer la vélocité et forcer l'activation
    bulletBody->setLinearVelocity(velocity);
    bulletBody->activate(true);
    
    // Configurer les flags de collision pour une balle réelle
    bulletBody->setCollisionFlags(bulletBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    
    // Ajouter la balle au monde physique
    dynamicsWorld->addRigidBody(bulletBody);
    
    // Forcer une mise à jour immédiate de la position
    btTransform transform;
    bulletBody->getMotionState()->getWorldTransform(transform);
    btVector3 pos = transform.getOrigin();
    bulletNode->setPosition(Vector3(pos.x(), pos.y(), pos.z()));
    
    // Stocker la balle
    bullets.push_back(std::make_pair(bulletBody, bulletNode));
}

void Player::renderDebug(btIDebugDraw* debugDrawer) {
    if (!playerBody || !debugDrawer) {
        return;
    }

    // Get the world transform of the player
    btTransform transform = playerBody->getWorldTransform();
    btVector3 origin = transform.getOrigin();
    btQuaternion rotation = transform.getRotation();

    // Draw the capsule shape
    btCapsuleShape* capsule = dynamic_cast<btCapsuleShape*>(playerBody->getCollisionShape());
    if (capsule) {
        btScalar radius = capsule->getRadius();
        btScalar height = capsule->getHalfHeight() * 2.0f; // Total height of capsule
        // Draw a wireframe capsule with Y-axis orientation (upAxis = 1)
        debugDrawer->drawCapsule(radius, height, 1, transform, btVector3(1, 0, 0)); // Red color
    } else {
        // Fallback to drawing a sphere if shape is not a capsule
        debugDrawer->drawSphere(origin, 0.5f, btVector3(1, 0, 0));
    }
}

void Player::updateBulletPositions(btDiscreteDynamicsWorld* dynamicsWorld) {
    for (size_t i = 0; i < bullets.size(); /* increment handled in loop */) {
        btRigidBody* bulletBody = bullets[i].first;
        SceneNode* bulletNode = bullets[i].second;

        if (bulletBody && bulletNode) {
            btTransform transform;
            bulletBody->getMotionState()->getWorldTransform(transform);
            btVector3 pos = transform.getOrigin();
            bulletNode->setPosition(Vector3(pos.x(), pos.y(), pos.z()));

            // Calculate distance from player
            Vector3 bulletPos(pos.x(), pos.y(), pos.z());
            Vector3 playerPos = playerNode->getPosition();
            float distance = bulletPos.distance(playerPos);

            // If bullet is too far, remove it
            if (distance > BULLET_MAX_DISTANCE) {
                // Remove bullet from physics world
                bulletBody->getMotionState()->getWorldTransform(transform);
                dynamicsWorld->removeRigidBody(bulletBody);
                delete bulletBody->getMotionState();
                delete bulletBody->getCollisionShape();
                delete bulletBody;

                // Remove bullet from scene
                bulletNode->getCreator()->destroySceneNode(bulletNode);

                // Remove from vector
                bullets.erase(bullets.begin() + i);
            } else {
                ++i; // Only increment if we didn't remove the bullet
            }
        } else {
            ++i; // Skip invalid bullets
        }
    }
}

const std::vector<std::pair<btRigidBody*, SceneNode*>>& Player::getBullets() const {
    return bullets;
}

void Player::removeBullet(size_t index, btDiscreteDynamicsWorld* dynamicsWorld) {
    if (index >= bullets.size()) return;

    // Extract bullet components
    btRigidBody* bulletBody = bullets[index].first;
    SceneNode* bulletNode = bullets[index].second;

    // Remove the bullet from the physics world
    dynamicsWorld->removeRigidBody(bulletBody);
    delete bulletBody->getMotionState();
    delete bulletBody;

    // Remove the bullet from the scene
    bulletNode->getCreator()->destroySceneNode(bulletNode);

    // Remove the bullet from the vector
    bullets.erase(bullets.begin() + index);
}

void Player::updateAnimations(float deltaTime) {
    if (!playerEntity || !currentAnimation) return;
    
    // Mise à jour simple de l'animation courante
    currentAnimation->addTime(deltaTime);
}

void Player::setAnimation(const std::string& animName) {
    if (!playerEntity) return;

    // Si l'animation demandée est déjà en cours, ne rien faire
    if (currentAnimation && currentAnimation->getAnimationName() == animName) {
        return;
    }

    // Vérifier si l'animation existe
    if (!playerEntity->hasAnimationState(animName)) {
        std::cout << "Warning: Animation '" << animName << "' not found." << std::endl;
        return;
    }

    // Désactiver l'animation courante si elle existe
    if (currentAnimation) {
        currentAnimation->setEnabled(false);
    }

    // Activer la nouvelle animation
    currentAnimation = playerEntity->getAnimationState(animName);
    currentAnimation->setEnabled(true);
    currentAnimation->setLoop(true);
    currentAnimation->setTimePosition(0);  // Repartir du début de l'animation
}

void Player::takeDamage(float damage) {
    float currentTime = Ogre::Root::getSingleton().getTimer()->getMilliseconds() / 1000.0f;
    
    // Vérifier le délai entre les dégâts
    if (currentTime - lastDamageTime < DAMAGE_COOLDOWN) {
        return;
    }
    
    health = std::max(0.0f, health - damage);
    lastDamageTime = currentTime;
    healthRegenDelay = HEALTH_REGEN_WAIT_TIME;  // Réinitialiser le délai de régénération
}

void Player::updateHealth(float deltaTime) {
    if (healthRegenDelay > 0) {
        healthRegenDelay -= deltaTime;
        return;
    }

    // Si la vie n'est pas au maximum et que le délai est écoulé, régénérer
    if (health < maxHealth) {
        health = std::min(maxHealth, health + HEALTH_REGEN_RATE * deltaTime);
    }
}

void Player::useEnergy(float amount) {
    energy = std::max(0.0f, energy - amount);
}

void Player::regenerateEnergy(float amount) {
    if (energy < maxEnergy) {
        energy = std::min(maxEnergy, energy + amount * energyRegenRate);
    }
}

void Player::resetPlayer() {
    // Réinitialiser la vie et l'énergie
    health = maxHealth;
    energy = maxEnergy;
    healthRegenDelay = 0.0f;
    lastDamageTime = 0.0f;
    
    // Réinitialiser la position du joueur
    if (playerBody) {
        btTransform transform;
        transform.setIdentity();
        transform.setOrigin(btVector3(0, 20.0f, 0));  // Légèrement au-dessus du sol
        playerBody->setWorldTransform(transform);
        playerBody->setLinearVelocity(btVector3(0, 0, 0));
        playerBody->setAngularVelocity(btVector3(0, 0, 0));
        playerBody->clearForces();
        playerBody->activate(true);  // S'assurer que le corps est actif
    }
    
    if (playerNode) {
        playerNode->setPosition(0, 0, 0);
        playerNode->setOrientation(Quaternion(Degree(90), Vector3::UNIT_Y));
    }

    // Réinitialiser l'animation
    if (playerEntity && playerEntity->hasAnimationState("Idle")) {
        setAnimation("Idle");
    }
}