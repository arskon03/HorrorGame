#pragma once

#include "DrawDebugHelpers.h"

/** This is a macro that will draw a basic red sphere at the location specified for the duration specified */
#define DRAW_BASIC_SPHERE(Location, Duration) \
if (GetWorld() != nullptr) \
{ \
	DrawDebugSphere(GetWorld(), Location, 15.f, 12, FColor::Red, false, Duration, 0U, 1); \
}

/** This is a macro that will draw a persistent red sphere at the location specified */
#define DRAW_PERSISTENT_SPHERE(Location) \
if (GetWorld() != nullptr) \
{ \
	DrawDebugSphere(GetWorld(), Location, 15.f, 12, FColor::Red, true, -1.F, 0U, 1); \
}

/** This is a macro that will draw a basic red line from the start to end location specified for the duration specified */
#define DRAW_BASIC_LINE(StartLocation, EndLocation, Duration) \
if (GetWorld() != nullptr) \
{ \
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, Duration, 0U, 1); \
}

/** This is a macro that will draw a persistent red line from the start to end location specified */
#define DRAW_PERSISTENT_LINE(StartLocation, EndLocation) \
if (GetWorld() != nullptr) \
{ \
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.F, 0U, 1); \
}

/** This is a macro that will draw a basic red point at the location specified for the duration specified */
#define DRAW_BASIC_POINT(Location, Duration) \
if (GetWorld() != nullptr) \
{ \
	DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, false, Duration); \
}

/** This is a macro that will draw a persistent red point at the location specified */
#define DRAW_PERSISTENT_POINT(Location) \
if (GetWorld() != nullptr) \
{ \
	DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, true, -1.F); \
}

/** This is a macro that will draw a basic red vector from the start to end location specified for the duration specified */
#define DRAW_BASIC_VECTOR(StartLocation, EndLocation, Duration) \
if (GetWorld() != nullptr) \
{ \
	DrawDebugDirectionalArrow(GetWorld(), StartLocation, EndLocation, 15.f, FColor::Red, false, Duration, 0U, 1); \
}

/** This is a macro that will draw a persistent red vector from the start to end location specified */
#define DRAW_PERSISTENT_VECTOR(StartLocation, EndLocation) \
if (GetWorld() != nullptr) \
{ \
	DrawDebugDirectionalArrow(GetWorld(), StartLocation, EndLocation, 15.f, FColor::Red, true, -1.F, 0U, 1); \
}
