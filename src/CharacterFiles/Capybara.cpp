#include "Capybara.h"

void Capy::Update()
{
    controller.Update(physBody);
    pos = GetPosition();
}

void Capy::Draw()
{
    DrawRectanglePro((Rectangle){pos.x, pos.y, width, height}, {width / 2, height / 2}, 0, BROWN);
}

Vector2 Capy::GetPosition()
{
    b2Vec2 tempPos = physBody->GetPosition();
    return {tempPos.x, tempPos.y};
}