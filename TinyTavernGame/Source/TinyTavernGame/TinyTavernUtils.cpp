// TinyTavern


#include "TinyTavernUtils.h"

bool AreVectorsSimilar(FVector Vector1, FVector Vector2)
{
    float EpsilonX = FMath::Max(FMath::Abs(Vector1.X), FMath::Abs(Vector2.X)) * 1E-2;
    float EpsilonY = FMath::Max(FMath::Abs(Vector1.Y), FMath::Abs(Vector2.Y)) * 1E-2;;
    float EpsilonZ = FMath::Max(FMath::Abs(Vector1.Z), FMath::Abs(Vector2.Z)) * 1E-2;;

    if (FMath::Abs(Vector1.X - Vector2.X) > EpsilonX)
    {
        return false;
    }

    if (FMath::Abs(Vector1.Y - Vector2.Y) > EpsilonY)
    {
        return false;
    }

    if (FMath::Abs(Vector1.Z - Vector2.Z) > EpsilonZ)
    {
        return false;
    }


    return true;
}
