// Fill out your copyright notice in the Description page of Project Settings.
#include "VMotionControllerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "NavigationSystem.h"

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

	if (TraceResult.HitResult.bBlockingHit)
	{
		FNavLocation ProjectedNavMeshLocation;
		if (UNavigationSystemV1::GetNavigationSystem(this)->ProjectPointToNavigation(TraceResult.HitResult.ImpactPoint, ProjectedNavMeshLocation))
		{
			TraceResult.HitResult.ImpactPoint = FVector(ProjectedNavMeshLocation);
		}
		else
		{
			TraceResult.HitResult.bBlockingHit = false;
		}
	}
	TArray<FVector> Positions;
	for (const FPredictProjectilePathPointData& Pos : TraceResult.PathData)
	{
		Positions.Add(Pos.Location);
	}
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(TraceComp, "RibbonPositions", Positions);
}

FHitResult UVMotionControllerComponent::StopGetTracingResult() const
{
	return TraceResult.HitResult;
}

void UVMotionControllerComponent::BeginPlay()
{
	Super::BeginPlay();
	TraceComp = NewObject<UNiagaraComponent>(GetOwner(), NAME_None);
	TraceComp->SetAsset(TraceSystemAsset);
	TraceComp->SetupAttachment(this);
	TraceComp->RegisterComponent();
	//TraceComp->SetHiddenInGame(true);
}
