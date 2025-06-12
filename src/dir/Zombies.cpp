#include "../include/Zombies.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <chrono>
#include <sstream>

namespace {
    // Static counter for unique zombie IDs
    static unsigned long long zombieCounter = 0;
    
    // Function to generate unique zombie names
    std::string generateUniqueName() {
        auto now = std::chrono::system_clock::now();
        auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
        auto value = now_ms.time_since_epoch();
        std::stringstream ss;
        ss << "Zombie_" << value.count() << "_" << ++zombieCounter;
        return ss.str();
    }
}

Zombies::Zombies() 
    : gameOverlay(nullptr)
    , overlayContainer(nullptr)
    , messageText(nullptr)
    , messageDisplayTimeRemaining(0.0f)
{
    initializeOverlay();
}

Zombies::~Zombies() {
    // Clean up overlay system
    if (messageText) {
        Ogre::OverlayManager::getSingleton().destroyOverlayElement(messageText);
    }
    if (overlayContainer) {
        Ogre::OverlayManager::getSingleton().destroyOverlayElement(overlayContainer);
    }
    if (gameOverlay) {
        Ogre::OverlayManager::getSingleton().destroy(gameOverlay);
    }

    // Clean up zombie bodies and nodes
    for (auto body : zombieBodies) {
        if (body) {
            delete body->getMotionState();
            delete body->getCollisionShape();
            delete body;
        }
    }
    zombieBodies.clear();
    zombieNodes.clear();
    zombieEntities.clear();
    zombieHealth.clear();
}

void Zombies::initializeOverlay() {
    try {
        // Create overlay
        Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
        gameOverlay = overlayManager.create("GameMessageOverlay");

        // Create container
        overlayContainer = static_cast<Ogre::OverlayContainer*>(
            overlayManager.createOverlayElement("Panel", "MessageContainer"));
        overlayContainer->setMetricsMode(Ogre::GMM_RELATIVE);
        overlayContainer->setPosition(0, 0);
        overlayContainer->setDimensions(1.0, 1.0);

        // Create text area
        messageText = static_cast<Ogre::TextAreaOverlayElement*>(
            overlayManager.createOverlayElement("TextArea", "MessageText"));
        messageText->setMetricsMode(Ogre::GMM_RELATIVE);
        messageText->setPosition(0.0, 0.3);  // Position verticale à 30% du haut
        messageText->setDimensions(1.0, 0.4);  // Hauteur de 40% de l'écran
        messageText->setCharHeight(0.15);  // Taille relative plus grande
        messageText->setAlignment(Ogre::TextAreaOverlayElement::Center);
        messageText->setHorizontalAlignment(Ogre::GHA_CENTER);
        messageText->setVerticalAlignment(Ogre::GVA_CENTER);
        messageText->setFontName("SdkTrays/Value");
        messageText->setColour(Ogre::ColourValue(1.0, 1.0, 1.0, 1.0));

        // Add elements to overlay
        overlayContainer->addChild(messageText);
        gameOverlay->add2D(overlayContainer);
        gameOverlay->setZOrder(300);
        gameOverlay->show();

        std::cout << "Overlay system initialized successfully" << std::endl;

    } catch (const Ogre::Exception& e) {
        std::cerr << "Failed to initialize overlay system: " << e.what() << std::endl;
    }
}

void Zombies::showKillLogMessage(const std::string& message, float displayTime) {
    if (!messageText || !gameOverlay) {
        std::cerr << "Error: Message text or overlay not initialized" << std::endl;
        return;
    }

    try {
        messageText->setCaption(message);
        messageText->setCharHeight(0.03);  // Petite taille pour les logs
        messageText->setPosition(0.7, 0.05);  // Position en haut à droite
        messageText->setDimensions(0.25, 0.1);  // Dimensions réduites
        messageDisplayTimeRemaining = displayTime;
        gameOverlay->show();
    } catch (const Ogre::Exception& e) {
        std::cerr << "Error showing kill log message: " << e.what() << std::endl;
    }
}

void Zombies::showGameMessage(const std::string& message, float displayTime, int fontSize) {
    if (!messageText || !gameOverlay) {
        std::cerr << "Error: Message text or overlay not initialized" << std::endl;
        return;
    }

    try {
        messageText->setCaption(message);
        float relativeSize = static_cast<float>(fontSize) / 500.0f;
        messageText->setCharHeight(relativeSize);
        messageText->setPosition(0.0, 0.3);  // Position centrale pour les messages de niveau
        messageText->setDimensions(1.0, 0.4);  // Dimensions normales
        messageDisplayTimeRemaining = displayTime;
        gameOverlay->show();
    } catch (const Ogre::Exception& e) {
        std::cerr << "Error showing game message: " << e.what() << std::endl;
    }
}

void Zombies::updateOverlay(float deltaTime) {
    if (!gameOverlay) return;

    if (messageDisplayTimeRemaining > 0) {
        messageDisplayTimeRemaining -= deltaTime;
        if (messageDisplayTimeRemaining <= 0) {
            gameOverlay->hide();
        }
    }
}

void Zombies::createZombies(Ogre::SceneManager* scnMgr, int numZombies, float radius, btDiscreteDynamicsWorld* dynamicsWorld) {
    std::srand(static_cast<unsigned>(std::time(0)));

    for (int i = 0; i < numZombies; ++i) {
        float x = static_cast<float>(std::rand()) / RAND_MAX * radius * 2 - radius;
        float z = static_cast<float>(std::rand()) / RAND_MAX * radius * 2 - radius;
        float y = 0.0f;

        Ogre::Vector3 position(x, y, z);

        // Create visual representation of the zombie with unique name
        std::string uniqueName = generateUniqueName();
        Ogre::Entity* zombieEntity = nullptr;
        try {
            zombieEntity = scnMgr->createEntity(uniqueName, "ZombieGirl_Body.mesh");
            if (!zombieEntity) {
                std::cerr << "Failed to create zombie entity: " << uniqueName << std::endl;
                continue;
            }
        } catch (const Ogre::Exception& e) {
            std::cerr << "Exception creating zombie entity: " << e.what() << std::endl;
            continue;
        }

        Ogre::SceneNode* zombieNode = scnMgr->getRootSceneNode()->createChildSceneNode(position);
        zombieNode->attachObject(zombieEntity);
        zombieNode->setScale(0.5, 0.5, 0.5);
        zombieNode->setPosition(position);

        // Rotation initiale pour faire face au centre
        Vector3 toCenter = Vector3(0, 0, 0) - position;
        toCenter.y = 0;
        if (toCenter.length() > 0.1f) {
            Quaternion rotation = Vector3::UNIT_Z.getRotationTo(toCenter);
            zombieNode->setOrientation(rotation);
        }

        zombieEntity->setCastShadows(true);

        AnimationState* zombieAnimation = zombieEntity->getAnimationState("my_animation");
        if (zombieAnimation) {
            zombieAnimation->setEnabled(true);
            zombieAnimation->setLoop(true);
        }

        zombieNodes.push_back(zombieNode);
        zombieEntities.push_back(zombieEntity);

        // Create physics for the zombie
        btCollisionShape* zombieShape = new btCapsuleShape(10.0f, 70.0f);
        btTransform zombieTransform;
        zombieTransform.setIdentity();
        zombieTransform.setOrigin(btVector3(position.x, position.y + 1.0f, position.z));

        // Set initial rotation in physics world
        Quaternion orientation = zombieNode->getOrientation();
        zombieTransform.setRotation(btQuaternion(orientation.x, orientation.y, 
                                                orientation.z, orientation.w));

        btScalar mass = 50.0f;
        btVector3 localInertia(0, 0, 0);
        zombieShape->calculateLocalInertia(mass, localInertia);
        btDefaultMotionState* motionState = new btDefaultMotionState(zombieTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, zombieShape, localInertia);
        btRigidBody* zombieBody = new btRigidBody(rbInfo);

        zombieBody->setAngularFactor(btVector3(0, 1, 0));
        dynamicsWorld->addRigidBody(zombieBody);
        zombieBodies.push_back(zombieBody);

        // Initialize zombie health
        zombieHealth.push_back(baseZombieHealth * healthMultiplier);
    }
}

void Zombies::updateZombies(Ogre::SceneNode* playerNode, float deltaTime) {
    if (!playerNode) return;

    Ogre::Vector3 playerPos = playerNode->getPosition();

    for (size_t i = 0; i < zombieBodies.size(); ++i) {
        if (!isZombieAlive(i) || !zombieBodies[i] || !zombieNodes[i]) continue;

        btVector3 zombiePos = zombieBodies[i]->getWorldTransform().getOrigin();
        Vector3 zombiePosition(zombiePos.x(), zombiePos.y(), zombiePos.z());
        Vector3 direction = playerPos - zombiePosition;
        direction.y = 0; // Garder le zombie droit
        
        if (direction.length() > 1.0f) {
            // Normaliser la direction pour le mouvement
            Vector3 normalizedDirection = direction.normalisedCopy();
            Vector3 velocity = normalizedDirection * ZOMBIE_SPEED * speedMultiplier;
            zombieBodies[i]->setLinearVelocity(btVector3(velocity.x, 0, velocity.z));
            
            // Faire face au joueur
            Quaternion zombieRotation;
            // Calculer l'angle entre l'axe Z et la direction vers le joueur
            zombieRotation = Vector3::UNIT_Z.getRotationTo(direction);
            
            // Appliquer la rotation au zombie
            zombieNodes[i]->setOrientation(zombieRotation);
            
            // Mettre à jour la rotation dans le monde physique
            btTransform transform = zombieBodies[i]->getWorldTransform();
            transform.setRotation(btQuaternion(zombieRotation.x, zombieRotation.y, 
                                             zombieRotation.z, zombieRotation.w));
            zombieBodies[i]->setWorldTransform(transform);
        }
        
        // Update visual position
        btTransform trans;
        zombieBodies[i]->getMotionState()->getWorldTransform(trans);
        zombieNodes[i]->setPosition(trans.getOrigin().x(), 0, trans.getOrigin().z());

        // Update animation
        if (zombieEntities[i]) {
            Ogre::AnimationState* zombieAnimation = zombieEntities[i]->getAnimationState("my_animation");
            if (zombieAnimation) {
                zombieAnimation->addTime(deltaTime * 0.5f);
            }
        }
    }
}

void Zombies::setHealthMultiplier(float multiplier) {
    healthMultiplier = multiplier;
    for (size_t i = 0; i < zombieHealth.size(); ++i) {
        if (isZombieAlive(i)) {
            zombieHealth[i] = baseZombieHealth * healthMultiplier;
        }
    }
}

void Zombies::setSpeedMultiplier(float multiplier) {
    speedMultiplier = multiplier;
}

bool Zombies::isZombieAlive(size_t index) const {
    if (index >= zombieHealth.size()) return false;
    return zombieHealth[index] > 0;
}

void Zombies::onBulletHit(size_t zombieIndex, float damage, btDiscreteDynamicsWorld* dynamicsWorld) {
    if (zombieIndex >= zombieHealth.size() || !isZombieAlive(zombieIndex)) return;

    zombieHealth[zombieIndex] -= damage;
    
    if (zombieHealth[zombieIndex] <= 0) {
        // Zombie is dead
        if (zombieBodies[zombieIndex]) {
            dynamicsWorld->removeRigidBody(zombieBodies[zombieIndex]);
            delete zombieBodies[zombieIndex]->getMotionState();
            delete zombieBodies[zombieIndex]->getCollisionShape();
            delete zombieBodies[zombieIndex];
            zombieBodies[zombieIndex] = nullptr;
        }
        
        if (zombieNodes[zombieIndex]) {
            zombieNodes[zombieIndex]->getCreator()->destroySceneNode(zombieNodes[zombieIndex]);
            zombieNodes[zombieIndex] = nullptr;
        }

        std::stringstream ss;
        ss << "Zombie " << zombieIndex << " éliminé!";
        showKillLogMessage(ss.str(), 2.0f);  // Affichage pendant 2 secondes
    } else {
        std::stringstream ss;
        ss << "Zombie " << zombieIndex << " touché! Vie: " << zombieHealth[zombieIndex];
        showKillLogMessage(ss.str(), 1.0f);  // Affichage pendant 1 seconde
    }
}

const std::vector<btRigidBody*>& Zombies::getZombieBodies() const {
    return zombieBodies;
}