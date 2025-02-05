// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/MathFwd.h"
#include "FlockAgent.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class AFlock;

UCLASS()
class TECENTDEMO_API AFlockAgent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlockAgent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void MoveTo();

	void SetUpFlock(AFlock *belongFlock);

private:

	UFUNCTION(BlueprintCallable)
	FVector CalculateDirection() const;

	UFUNCTION(BlueprintCallable)
	FVector CalculateCohesionDirection(const TArray<FHitResult> &traceResults) const;

	UFUNCTION(BlueprintCallable)
	FVector CalculateAlignmentDirection(const TArray<FHitResult> &traceResults) const;

	UFUNCTION(BlueprintCallable)
	FVector CalculateAvoidanceDirection(const TArray<FHitResult> &traceResults) const;

	UFUNCTION(BlueprintCallable)
	FVector CalculateLimitedMotionDirection() const;
	
private:
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent *m_pBoxComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *m_pStaticMesh;
	
	UPROPERTY(VisibleAnywhere)
	AFlock *m_pFlock;
	
	UPROPERTY(EditAnywhere)
	float m_cohesionRadius;

	UPROPERTY(EditAnywhere)
	float m_avoidanceRadius;

	UPROPERTY(EditAnywhere)
	float m_rotateSpeed;

	UPROPERTY(EditAnywhere)
	float m_moveSpeed;

	/*
	 *	Factors
	 */
	UPROPERTY(EditAnywhere)
	float m_cohesionFactor;

	UPROPERTY(EditAnywhere)
	float m_avoidanceFactor;

	UPROPERTY(EditAnywhere)
	float m_alignmentFactor;

	UPROPERTY(EditAnywhere)
	float m_limitedMotionFactor;
	
};
