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
	
	//��Ÿ�� ���� ����
	bool IsDash;
	
	float OriginSpeed;


public:

	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//���� �� �ӵ�
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float DashSpeed;

	//���� ���� �ð�
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float DashDuration;

	//���� ��Ÿ��
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float DashCooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float SpeedIncreaseRate;


	//������ �� ������Ʈ�� �÷��̾� �ڿ� �ִ� ī�޶� ���� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USpringArmComponent* SpringArmComp;

	//�÷��̾ ī�޶� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UCameraComponent* CameraComp;

	//��, �ڷ� �̵��ϴ� �Լ�
	void MoveForward(float InputAxis);

	//��, ��� �̵��ϴ� �Լ�
	void MoveRight(float InputAxis);

	//�޸��� ���� �Լ�
	void BeginSprint();

	//�޸��� ���� �Լ�
	void EndSprint();

	//���� �Է� �Լ�
	void DashInput();

	//���� ���� �Լ�
	void Dash();

	//���� ���� �Լ�
	void EndDash();

	//���� ��Ÿ�� �缳�� �Լ�
	void ResetDashCooldown();

	//�̵��ӵ� ���� �Լ�
	void IncreaseDashSpeed(float DeltaTime) const;

	//���� ��Ÿ�� ó�� ����
	FTimerHandle DashCooldownTimerHandle;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
