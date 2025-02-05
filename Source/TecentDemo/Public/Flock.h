// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Array.h"
#include "Flock.generated.h"

class AFlockAgent;

UCLASS()
class TECENTDEMO_API AFlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlock();

	const FBox& GetMotionRange() const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere)
	FBox m_motionRange;
	
	UPROPERTY(EditAnywhere)
	float m_flockRadius;
	
	UPROPERTY(EditAnywhere)
	float m_agentCount;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *m_pStaticMesh;
	
	UPROPERTY(VisibleAnywhere)
	TArray<AFlockAgent *> m_flockAgentArray;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AFlockAgent> m_flockAgentClass;	
};
