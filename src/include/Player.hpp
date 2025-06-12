#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "lib.hpp"
#include <vector>
#include <utility>

// Constantes pour les balles
// BULLET_SPEED est défini dans lib.hpp
const float BULLET_MAX_DISTANCE = 1000.0f;  // Distance maximale de rendu pour les balles

// Constantes pour la santé et l'énergie
const float HEALTH_REGEN_DELAY = 1.0f;      // Délai avant la régénération de la vie
const float ENERGY_REGEN_RATE = 10.0f;      // Taux de régénération de l'énergie

// Constantes pour la gravité
const btScalar GRAVITY = -9.8f;             // Gravité standard

// Constantes pour la physique
const btScalar BULLET_MASS = 0.01f;         // Masse légère pour les balles
const btScalar PLAYER_MASS = 150.0f;        // Masse du joueur

// Constantes pour les collisions
const float COLLISION_RADIUS = 0.03f;       // Rayon de collision pour les balles

// Constantes pour les animations
const float ANIMATION_SPEED = 1.0f;         // Vitesse d'animation

// Constantes pour le mouvement
const float MOVE_SPEED = 10.0f;             // Vitesse de déplacement
const float JUMP_FORCE = 30.0f;             // Force du saut

class Player {
public:
    Player();
    ~Player();

    int posX;
    int posZ;
    MaterialPtr material;
    int health;
    int speed;
    int damage;
    SceneNode* playerNode;
    Entity* playerEntity;
    Entity* bulletEntity;
    SceneNode* bulletNode;
    btRigidBody* bulletBody;
    AnimationState* playerAnimation;
    AnimationState* currentAnimation;

    void createPlayer(SceneManager* scnMgr, Vector3 position, btDiscreteDynamicsWorld* dynamicsWorld);
    void renderDebug(btIDebugDraw* debugDrawer);
    btRigidBody* playerBody; // Add playerBody for physics
    void shoot(SceneManager* scnMgr, btDiscreteDynamicsWorld* dynamicsWorld, Vector3 direction);
    void updateBulletPositions(btDiscreteDynamicsWorld* dynamicsWorld);
    void createCamera(SceneManager* scnMgr , Camera * cam); // Create camera method

    // Animation methods
    void updateAnimations(float deltaTime);
    void setAnimation(const std::string& animationName);

    // New methods
    const std::vector<std::pair<btRigidBody*, SceneNode*>>& getBullets() const; // Getter for bullets
    void removeBullet(size_t index, btDiscreteDynamicsWorld* dynamicsWorld);

    // Fonctions pour la santé et l'énergie
    void takeDamage(float damage);
    void useEnergy(float amount);
    void regenerateEnergy(float amount);
    void updateHealth(float deltaTime);  // Nouvelle fonction pour gérer la régénération de vie
    bool hasEnoughEnergy() const { return energy > 0.0f; }
    float getHealth() const { return health; }
    float getEnergy() const { return energy; }
    bool isAlive() const { return health > 0; }
    void resetPlayer();

private:
    // Store bullets for cleanup
    std::vector<std::pair<btRigidBody*, SceneNode*>> bullets;  // Changed from SceneNode*, Vector3 to btRigidBody*, SceneNode*
    static constexpr float BULLET_MAX_DISTANCE = 1000.0f;

    // Système de santé et d'énergie
    float maxHealth;
    float energy;
    float maxEnergy;
    float energyRegenRate;
    float lastDamageTime;  // Pour le délai entre les dégâts
    float healthRegenDelay;  // Délai avant la régénération de vie
    static constexpr float DAMAGE_COOLDOWN = 0.5f;  // Délai de 0.5s entre les dégâts
    static constexpr float HEALTH_REGEN_WAIT_TIME = 7.0f;  // Délai de 7s avant la régénération
    static constexpr float HEALTH_REGEN_RATE = 10.0f;  // Taux de régénération de vie par seconde
    static constexpr float PLAYER_BULLET_SPEED = 1000.0f; // Vitesse des balles en unités par seconde
};

#endif