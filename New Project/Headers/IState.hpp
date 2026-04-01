#ifndef ISTATE_HPP
#define ISTATE_HPP

class GameManager; // Forward Declaration

class IState {
public:
    virtual ~IState() {}
    virtual void Enter(GameManager& gm) = 0;
    virtual void Update(GameManager& gm) = 0;
    virtual void Draw(GameManager& gm) = 0;
};
#endif