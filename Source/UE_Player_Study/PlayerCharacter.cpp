// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter() 
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 클래스 컴포넌트 인스턴스화
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));


	//캐릭터 메시 트랜스폼의 위치와 회전을 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FQuat(FRotator(0.0f, -90.0f, 0.0f)));

	// 클래스 컴포넌트를 디폴트 캐릭터의 스켈레탈 메시 컴포넌트에 장착
	SpringArmComp->SetupAttachment(GetMesh());

	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	//스프링 암의 클래스 변수를 설정합니다.

	SpringArmComp->bUsePawnControlRotation = true;

	//캐릭터 무브먼트 컴포넌트의 클래스 변수를 설정합니다.

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	GetCharacterMovement()->bIgnoreBaseRotation = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerCharacter::MoveForward(float AxisValue)
{
    if ((Controller != nullptr) && (AxisValue != 0.0f))
    {
        // 앞쪽 찾기
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // 앞쪽 벡터 구하기
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, AxisValue);
    }
}

void APlayerCharacter::MoveRight(float AxisValue)
{
    if ((Controller != nullptr) && (AxisValue != 0.0f))
    {
        // 오른쪽 찾기
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // 오른쪽 벡터 구하기 
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // 해당 방향으로 이동 추가
        AddMovementInput(Direction, AxisValue);
    }
}

void APlayerCharacter::BeginSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}

void APlayerCharacter::EndSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = 600.f;
}



// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);


    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::BeginSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::EndSprint);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

}


