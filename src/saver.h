#ifndef SAVER_H_
#define SAVER_H_

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <Box2D/Box2D.h>

class World;
class Matter;

/**
 * Saver manages game saving and loading
 */
class Saver
{
public:
    /// Singleton is for convenient. One can still instantiate this class
    static Saver &getInstance()
    {
        static Saver *instance = NULL;
        return instance ? *instance : *(instance = new Saver());
    }

    /// If the path should be dir1/dir2/save, path = {dir1, dir2, save}
    /// This enhanced cross-platform behavior
    void saveTo(const World *world, std::vector<std::string> path) noexcept;
    /// The path is as that in saveTo
    /// Returns false when fails
    bool loadFrom(World *world, std::vector<std::string> path, float offsetX = 0, int player = -1) noexcept;

    std::string makePath(std::vector<std::string> path);

private:
    enum MatterId
    {
        ID_SMALL_WOOD_BLOCK,
        ID_LARGE_WOOD_BLOCK,
        ID_SMALL_ENGINE,
        ID_LARGE_ENGINE,
        ID_SMALL_STEEL_BALL,
        ID_BOMB,
        ID_STEEL_STICK,
        ID_WOOD_STICK
    };
    MatterId getMatterId(const Matter *m);

    enum IOFlag
    {
        // not set to zero for the case of input failure
        IO_CONTINUE = 1,
        IO_STOP = 2
    };
    IOFlag getFlag(std::istream &is);

    void getBodyData(std::ostream &os, const b2Body *b);
    b2Body *setBodyData(std::istream &is, b2World *w, float offsetX = 0);

    void getFixtureData(std::ostream &os, const b2Fixture *f);
    b2Fixture *setFixtureData(std::istream &is, b2Body *b);

    void getJointData(std::ostream &os, const b2Joint *j);
    b2Joint *setJointData(std::istream &is, b2World *w);

    void getMatterData(std::ostream &os, const Matter *m);
    Matter *setMatterData(std::istream &is, World *world, b2Body *b, int player);

    std::unordered_map<const b2Body*, int> tempId; /// used while saving
    std::unordered_map<int, b2Body*> tempAddr; /// used while loading
};

#endif // SAVER_H_

