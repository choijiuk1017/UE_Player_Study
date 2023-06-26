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

	//�޸��� ����
	void BeginSprint();

	//�޸��� ��
	void EndSprint();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
