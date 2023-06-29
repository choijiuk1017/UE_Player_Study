// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"


UCLASS()
class UE_PLAYER_STUDY_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
private:
	bool IsJump;
	
	//쿨타임 관리 변수
	bool IsDash;
	
	float OriginSpeed;


public:

	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//돌진 시 속도
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float DashSpeed;

	//돌진 지속 시간
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float DashDuration;

	//돌진 쿨타임
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float DashCooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float SpeedIncreaseRate;


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

	//달리기 시작 함수
	void BeginSprint();

	//달리기 종료 함수
	void EndSprint();

	//돌진 입력 함수
	void DashInput();

	//돌진 실행 함수
	void Dash();

	//돌진 종료 함수
	void EndDash();

	//돌진 쿨타임 재설정 함수
	void ResetDashCooldown();

	//이동속도 증가 함수
	void IncreaseDashSpeed(float DeltaTime) const;

	//돌진 쿨타임 처리 변수
	FTimerHandle DashCooldownTimerHandle;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
