#pragma once
#include"collision.h"
#include"NPC.h"
class collisionmanager {
public:
    camera* player;
    std::vector<AABB*> items;
    std::vector<dinosaur*>animals;
    void load(camera& cam) {
        player = &cam;
    }
    void load(dinosaur& dino) {
        animals.push_back(&dino);
    }
    void load(AABB* aabb) {
        items.push_back(aabb);
    }
    void checkmove(float x, float z,bool &fire) {
        player->updatefrom(x, z);
        for (dinosaur* animal : animals) {
            if (player->box.check(animal->box)) {
                player->updatefrom(-x, -z);
            }
        }
        for (AABB* aabb : items) {
            if (player->box.check(*aabb)) {
                player->updatefrom(-x, -z);
            }
        }
        if (fire) {
            checkray();
        }
    }
    void checkray() {
        ray temp(player->from, player->forward.normalize());
        for (dinosaur* animal : animals) {
            if (raySphere(temp, animal->box)) {
                animal->health -= 50;
            }
        }
    }

};