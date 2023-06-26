// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class UE_PLAYER_STUDY_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//스프링 암 컴포넌트가 플레이어 뒤에 있는 카메라를 따라 감
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USpringArmComponent* SpringArmComp;

	//플레이어가 카메라를 따라감
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UCameraComponent* CameraComp;

	//앞, 뒤로 이동하는 함수
	void MoveForward(float InputAxis);

	//좌, 우로 이동하는 함수
	void MoveRight(float InputAxis);

	//달리기 시작
	void BeginSprint();

	//달리기 끝
	void EndSprint();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
