// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableAladinController.h"
#include  "Blueprint/UserWidget.h"
#include "AladinGameState.h"
#include "EnhancedInputSubsystems.h"//EnhancedInputSubsystem 관련 헤더파일 include
#include "Components/TextBlock.h"
#include "AladinGameInstance.h"
#include "Kismet/GameplayStatics.h"


APlayableAladinController::APlayableAladinController()
	:InputMappingContext(nullptr)
	, MoveAction(nullptr)
	, LookAction(nullptr)
	, JumpAction(nullptr)
	, SprintAction(nullptr)//생성자에서 초기화
	, HUDWidgetClass(nullptr)
	, HUDWidgetInstance(nullptr)
	, MainMenuWidgetClass(nullptr)
	, MainMenuWidgetInstance(nullptr)
{
}

void APlayableAladinController::BeginPlay()
{
	Super::BeginPlay();

	//현재 PlayerController에 연결된 local player 객체를 가져옵시다(LocalPlayer는 ULocalPlayer 클래스의 포인터)
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		//Local Player에서 EnhancedInputLocalPlayerSubsystem을 가져옵시다
		// (UEnhancedInputLocalPlayerSubsystem은 ULocalPlayer의 서브시스템, 또한 그것을 포인터 만들어서 
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				//서브시스템을 통해 우리가 할당한 IMC를 활성화
				//우선순위(Priority)는 0이 가장 높은 우선순위
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	AAladinGameState* AladinGamestate = GetWorld() ? GetWorld()->GetGameState<AAladinGameState>() : nullptr;
	if (AladinGamestate)
	{
		AladinGamestate->UpdateHUD();
	}

	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MenuLevel"))
	{
		ShowMainMenu(false);
	}

}


UUserWidget* APlayableAladinController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void APlayableAladinController::ShowMainMenu(bool bIsRestart)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetClass = nullptr;

	}
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetClass = nullptr;
	}
	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());//UI에만 인풋 가능하게 설정
		}
	}
	if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
	{
		UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("GameStartRestart"));
		if (PlayAnimFunc)
		{
			MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
		}
		if (bIsRestart)
		{
			ButtonText->SetText(FText::FromString(TEXT("Restart")));

		}
		else
		{
			ButtonText->SetText(FText::FromString(TEXT("Start")));
		}
	}
	if (bIsRestart)
	{
		UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
		if (PlayAnimFunc)
		{
			MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
		}
		if (UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("TotalScore")))
		{
			if (UAladinGameInstance* AladinGameInstance = Cast<UAladinGameInstance>(UGameplayStatics::GetGameInstance(this)))
			{
				TotalScoreText->SetText(FText::FromString(
					FString::Printf(TEXT("Total Score: %d"), AladinGameInstance->TotalScore)));
			}
		}
	}

}

void APlayableAladinController::ShowGameHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetClass = nullptr;

	}
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetClass = nullptr;
	}
	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;//마우스가 없어져야하니까
			SetInputMode(FInputModeGameOnly());//Game에만 인풋 가능하게 설정
		}
	}
	AAladinGameState* AladinGameState = GetWorld() ? GetWorld()->GetGameState<AAladinGameState>() : nullptr;
	AladinGameState->UpdateHUD();
}

void APlayableAladinController::StartGame()
{
	if (UAladinGameInstance* AladinGameInstance = Cast<UAladinGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		AladinGameInstance->CurrentLevelIndex = 0;
		AladinGameInstance->TotalScore = 0;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("Level1"));
	SetPause(true);

}