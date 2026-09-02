// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableAladin.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"//속도 관련 헤더 추가
#include "PlayableAladinController.h"
#include "EnhancedInputComponent.h"//Input Action과 함수를 연결(Bind)할 수 있게 해주는 헤더
#include "Components/WidgetComponent.h"
#include "AladinGameState.h"
#include "Components/TextBlock.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

APlayableAladin::APlayableAladin()
{
	// 이 캐릭터가 매 프레임마다 Tick을 호출하도록 설정
	PrimaryActorTick.bCanEverTick = true;
	//(1) 스프링 암 생성
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	// 스프링 암을 루트 컴포넌트 (CapsuleComponent)에 부착 
	SpringArmComp->SetupAttachment(RootComponent);
	// 캐릭터와 카메라 사이의 거리 기본값 300으로 설정
	SpringArmComp->TargetArmLength = 300.0f;
	// 컨트롤러 회전에 따라 스플링 암도 회전하도록 설정함
	SpringArmComp->bUsePawnControlRotation = true;

	//(2) 카메라 컴포넌트 생성
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	// 스프링 암의 소켓 위치에 카메라를 부착
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	// 카메라는 스프링 암의 회전을 따르므로 PawnControlRotation은 꺼둠
	CameraComp->bUsePawnControlRotation = false;
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);
	NormalSpeed = 600.f;
	SprintSpeedMultiplier = 1.5f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	MaxHealth = 1000.0f;
	Health = MaxHealth;
	DebuffStack = 0;
	DisableMoveTime = 0.0f;
}

// 게임 시작 시 호출
void APlayableAladin::BeginPlay()
{
	Super::BeginPlay();
	//Rotation 설정(Pitch=-24.404497,Yaw=57.267592,Roll=-32.732407)
	//모든 축을 배로 스케일
	SetActorScale3D(FVector(0.5f));
	//(X = , Y = , Z = )
	UpdateOverheadHP();


	if (GetWorld())
	{
	
		FString MapName = GetWorld()->GetMapName();

		MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	
		if (MapName.Equals(TEXT("MenuLevel"), ESearchCase::IgnoreCase))
		{
			APlayerController* PC = Cast<APlayerController>(GetController());

			if (PC && VideoWidgetClass)
			{
				
				VideoWidgetInstance = CreateWidget<UUserWidget>(PC, VideoWidgetClass);

				if (VideoWidgetInstance)
				{
					VideoWidgetInstance->AddToViewport();
				}
			}
		}
	}
}

void APlayableAladin::Tick(float DeltaTime)//매 프레임마다 호출
{
	Super::Tick(DeltaTime);

}

// 호출 하여 입력 기능을 묶기
void APlayableAladin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Enhanced InputComponent로 캐스팅
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//IA를 가져오기 위해 현재 소유 중인 Controller를 ASpartaPlayerController로 캐스팅
		if (APlayableAladinController* PlayerController = Cast<APlayableAladinController>(GetController()))
		{
			if (PlayerController->MoveAction)//IA_Move 액션 키를 누르고 있는 동안 Move()호출
			{
				//BindAction()은 쉽게 말하면 이 입력이 발생하면 묶은 것들이 모인 이 함수를 실행해!
				// 첫 번째 인자: 어떤 입력 액션을 사용할지 지정
				// 두 번째 인자: 어떤 시점에 함수를 실행할지 지정 
				// 세 번째 인자: 현재 객체(this)에서 함수를 실행하도록 지정
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &APlayableAladin::Move);
			}
			if (PlayerController->JumpAction)//IA_Jump 액션 키를 누르고 동안 StartJump()호출
			{
				EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, this, &APlayableAladin::StartJump);
			}
			if (PlayerController->LookAction)//IA_Look 액션 마우스가 Look()호출
			{
				EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &APlayableAladin::Look);
			}
			if (PlayerController->SprintAction)//IA_Sprint 액션키를 누르고 있는 동안 StartSprint() 호출
			{
				EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Triggered, this, &APlayableAladin::StartSprint);
			}
			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Completed, this, &APlayableAladin::StopSprint);
			}
		}


	}
}

void APlayableAladin::Move(const FInputActionValue& value)
{
	//컨트롤러가 있어야 방향 계산이 가능하겠죠?
	if (!Controller) return;
	//Value는 Axis2D로 설정된 IA_Move의 입력값(WASD)을 담고 있음
	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		// 캐릭터가 바라보는 방향으로 x축 이동
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		//캐릭터의 오른쪽 방향으로 Y축 이동
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}


void APlayableAladin::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();//캐릭터에서 기본으로 제공해줍니다.
	}
}
void APlayableAladin::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();//캐릭터에서 기본으로 제공해줍니다.
	}
}
void APlayableAladin::Look(const FInputActionValue& value)
{
	//마우스의 X,Y 움직임을 2D 축으로 가져옴
	FVector2D LookInput = value.Get<FVector2D>();
	//X는 좌우 회전(Yaw), Y는 상하 회전 (Pitch)
	//좌우 회전
	AddControllerYawInput(LookInput.X);
	//상하 회전
	AddControllerPitchInput(LookInput.Y);
}
void APlayableAladin::StartSprint(const FInputActionValue& value)
{
	//Shift키를 누른 순간 이 함수가 호출됨
	//달리기 속도를 조정
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void APlayableAladin::StopSprint(const FInputActionValue& value)
{
	//Shift 키를 땐 순간 이 함수 호출
	//기본 걷기로 복귀
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

float APlayableAladin::GetHealth() const
{
	return Health;
}

void APlayableAladin::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
	UpdateOverheadHP();
}

float APlayableAladin::TakeDamage(float DamageAmount,
	struct FDamageEvent const& DamageEvent, //어떤 데미지 이벤트인지 (불,얼음 등)
	AController* EventInstigator,//누가 데미지를 입혔는지
	AActor* DamageCauser)//데미지를 일으킨 오브젝트
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	UpdateOverheadHP();

	if (Health <= 0.0f)
	{
		OnDeath();
	}
	return ActualDamage;

}

void APlayableAladin::OnDeath()
{
	AAladinGameState* AladinGameState = GetWorld() ? GetWorld()->GetGameState<AAladinGameState>() : nullptr;
	if (AladinGameState)
	{
		AladinGameState->OnGameOver();
	}
}

void APlayableAladin::UpdateOverheadHP()
{
	if (!OverheadWidget) return;

	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) return;

	if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
	{
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth)));
	}
}
float APlayableAladin::GetSpeed()
{
	return NormalSpeed;
}

void APlayableAladin::SetSpeed(float normalspeed)
{
	NormalSpeed = normalspeed;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}
int32 APlayableAladin::GetDebuffStack() const
{
	return DebuffStack;
}

void APlayableAladin::AddDebuffStack()
{
	DebuffStack++;
}

float APlayableAladin::GetDisableMoveTime() const
{
	return DisableMoveTime;
}

void APlayableAladin::SetDisableMoveTime(float Time)
{
	DisableMoveTime = Time;
}