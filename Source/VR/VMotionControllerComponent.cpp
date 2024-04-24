// Fill out your copyright notice in the Description page of Project Settings.


#include "VMotionControllerComponent.h"
#include "Kismet/GameplayStatics.h"
void UVMotionControllerComponent::StartTracing()
{
}

void UVMotionControllerComponent::DoTracing()
{
	FPredictProjectilePathParams PathParams;
	PathParams.ActorsToIgnore.Add(GetOwner());
	PathParams.bTraceWithChannel = true;
	PathParams.TraceChannel = ECC_Visibility;
	PathParams.DrawDebugType = EDrawDebugTrace::ForOneFrame;
	PathParams.StartLocation = GetComponentLocation();
	PathParams.LaunchVelocity = GetForwardVector() * TraceProjectileLaunchVelocity;
	PathParams.bTraceWithCollision = true;
	PathParams.ProjectileRadius = TraceProjectileRadius;

	UGameplayStatics::PredictProjectilePath(this, PathParams, TraceResult);
}

FHitResult UVMotionControllerComponent::StopGetTracingResult() const
{
	return FHitResult();
}
