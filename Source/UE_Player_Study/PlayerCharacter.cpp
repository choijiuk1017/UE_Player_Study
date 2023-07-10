// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
//������ �Լ�
APlayerCharacter::APlayerCharacter() 
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Ŭ���� ������Ʈ �ν��Ͻ�ȭ
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));


	//ĳ���� �޽� Ʈ�������� ��ġ�� ȸ���� ����
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FQuat(FRotator(0.0f, -90.0f, 0.0f)));

	// Ŭ���� ������Ʈ�� ����Ʈ ĳ������ ���̷�Ż �޽� ������Ʈ�� ����
	SpringArmComp->SetupAttachment(GetMesh());

	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);


	//������ ���� Ŭ���� ������ ����
	SpringArmComp->bUsePawnControlRotation = true;

	//ĳ���� �����Ʈ ������Ʈ�� Ŭ���� ������ �����մϴ�.

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	GetCharacterMovement()->bIgnoreBaseRotation = true;

    //���� ���� Ȯ�� ���� �ʱ�ȭ
    IsJump = false;

    //�⺻ ���� �ӵ� ����
    DashSpeed = 3000.0f;

    //���� ���� �ð� ����
    DashDuration = 3.0f;

    //���� ��Ÿ�� ����
    DashCooldown = 20.0f;

    //���� ���� Ȯ�� ����
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

//��, �ڷ� �̵��ϴ� �Լ�
void APlayerCharacter::MoveForward(float AxisValue)
{
    if ((Controller != nullptr) && (AxisValue != 0.0f))
    {
        // ���� ã��
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // ���� ���� ���ϱ�
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, AxisValue);
    }
}

//��, ��� �̵��ϴ� �Լ�
void APlayerCharacter::MoveRight(float AxisValue)
{
    if ((Controller != nullptr) && (AxisValue != 0.0f))
    {
        // ������ ã��
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // ������ ���� ���ϱ� 
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // �ش� �������� �̵� �߰�
        AddMovementInput(Direction, AxisValue);
    }
}

//�޸��� ���� �Լ�
void APlayerCharacter::BeginSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = 1000.0f;

    OriginSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

//�޸��� ���� �Լ�
void APlayerCharacter::EndSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = 600.0f;

    OriginSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

//���� Ű �Է� �޴� �Լ�
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

//���� �Լ�
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

//���� ���� �Լ�
void APlayerCharacter::EndDash()
{
    GetCharacterMovement()->MaxWalkSpeed = OriginSpeed;
}

//���� ��Ÿ�� ���� �Լ�
void APlayerCharacter::ResetDashCooldown()
{
    // ��Ÿ���� ������ �� ���� ���� ���·� ����
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


