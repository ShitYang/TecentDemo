// Fill out your copyright notice in the Description page of Project Settings.


#include "AFish.h"

// Sets default values
AAFish::AAFish()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAFish::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAFish::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

