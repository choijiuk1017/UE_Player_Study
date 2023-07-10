// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
//생성자 함수
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


	//스프링 암의 클래스 변수를 설정
	SpringArmComp->bUsePawnControlRotation = true;

	//캐릭터 무브먼트 컴포넌트의 클래스 변수를 설정합니다.

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	GetCharacterMovement()->bIgnoreBaseRotation = true;

    //점프 상태 확인 변수 초기화
    IsJump = false;

    //기본 돌진 속도 설정
    DashSpeed = 3000.0f;

    //돌진 지속 시간 설정
    DashDuration = 3.0f;

    //돌진 쿨타임 설정
    DashCooldown = 20.0f;

    //돌진 가능 확인 변수
    IsDash = true;

    IsFalling = false;

    OriginSpeed = GetCharacterMovement()->MaxWalkSpeed;

    SpeedIncreaseRate = 500.0f;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

//앞, 뒤로 이동하는 함수
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

//좌, 우로 이동하는 함수
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

//달리기 시작 함수
void APlayerCharacter::BeginSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = 1000.0f;

    OriginSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

//달리기 종료 함수
void APlayerCharacter::EndSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = 600.0f;

    OriginSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

//돌진 키 입력 받는 함수
void APlayerCharacter::DashInput()
{
    if (IsDash)
    {
        Dash();
        IncreaseDashSpeed(GetWorld()->GetDeltaSeconds());
        IsDash = false;

        GetWorldTimerManager().SetTimer(DashCooldownTimerHandle, this, &APlayerCharacter::ResetDashCooldown, DashCooldown, false);
    }
}

//돌진 함수
void APlayerCharacter::Dash()
{
    float StartTime = GetWorld()->GetTimeSeconds();

    float EndTime = StartTime + DashDuration;
    GetCharacterMovement()->MaxWalkSpeed = 4000.0f;

    if (GetCharacterMovement()->MaxWalkSpeed == DashSpeed)
    {
        EndDash();
    }   
    
}

//돌진 종료 함수
void APlayerCharacter::EndDash()
{
    GetCharacterMovement()->MaxWalkSpeed = OriginSpeed;
}

//돌진 쿨타임 관련 함수
void APlayerCharacter::ResetDashCooldown()
{
    // 쿨타임이 끝났을 때 돌진 가능 상태로 설정
    IsDash = true;
}

void APlayerCharacter::IncreaseDashSpeed(float DeltaTime) const
{
    GetCharacterMovement()->MaxWalkSpeed += SpeedIncreaseRate * DeltaTime;
}

void APlayerCharacter::PlayFallingAnimation()
{
    ACharacter* PlayerCharacter = this;
    UAnimMontage* Montage = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/Animation/Falling_Montage"));
    PlayerCharacter->PlayAnimMontage(Montage);
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
    PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &APlayerCharacter::Dash);

}


