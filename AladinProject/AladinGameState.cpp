#include "AladinGameState.h"
#include "AladinGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Spawn_Volume.h"
#include "Coin_Item.h"
#include "PlayableAladinController.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "PlayableAladin.h"
#include "Spawn_VolumeForMonster.h"
#include "Spawn_VolumeFallingHands.h"

AAladinGameState::AAladinGameState()
    : Score(0)
    , SpawnedCoinCount(0)
    , CollectedCoinCount(0)
    , LevelDuration(79.9999f)//
    , CurrentLevelIndex(0)
    , MaxLevels(3)
    , ItemToSpawn(40)
	, ItemToSpawn2(500)
	
{
}

void AAladinGameState::BeginPlay()
{
	Super::BeginPlay();

	FString LevelName = UGameplayStatics::GetCurrentLevelName(this);

	if (LevelName == TEXT("MenuLevel"))
	{
		if (APlayableAladin* Aladin = Cast<APlayableAladin>(
			UGameplayStatics::GetPlayerPawn(this, 0)))
		{
			Aladin->SetActorHiddenInGame(true);
		}
	}
	else
	{
		StartLevel();

		GetWorldTimerManager().SetTimer(
			HUDUpdateTimerHandle,
			this,
			&AAladinGameState::UpdateHUD,
			0.1f,
			true);
		GetWorldTimerManager().SetTimer(
			SpawnTimerHandle,
			this,
			&AAladinGameState::SpawnItems,
			SpawnCycle,
			true
		);
		GetWorldTimerManager().SetTimer(
			FallingHandsTimer,
			this,
			&AAladinGameState::SpawnFallingHands,
			SpawnCycle2,
			true
		);
	}
}


int32 AAladinGameState::GetScore() const
{
	return Score;
}
void AAladinGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UAladinGameInstance* AladinGameInstance = Cast<UAladinGameInstance>(GameInstance);
		if (AladinGameInstance)
		{
			AladinGameInstance->AddToScore(Amount);
		}
	}
}

void AAladinGameState:: StartLevel()
{
	WaveCount = 1;

	UE_LOG(LogTemp, Warning, TEXT("Wave %d Start!"), WaveCount);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Green,
			TEXT("Wave 1 Start!")
		);
	}
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (APlayableAladinController* AladinPlayerController = Cast<APlayableAladinController>(PlayerController))
		{
			AladinPlayerController->ShowGameHUD();
		}
	}


	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UAladinGameInstance* AladinGameInstance = Cast<UAladinGameInstance>(GameInstance);
		if (AladinGameInstance)
		{
			CurrentLevelIndex = AladinGameInstance->CurrentLevelIndex;
		}
	}

	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawn_Volume::StaticClass(), FoundVolumes);
	TArray<AActor*> FoundMonsterVolumes;

	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		ASpawn_VolumeForMonster::StaticClass(),
		FoundMonsterVolumes
	);
	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawn_Volume* SpawnVolume = Cast<ASpawn_Volume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoin_Item::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
		if (FoundMonsterVolumes.Num() > 0)
		{
			ASpawn_VolumeForMonster* MonsterSpawnVolume =
				Cast<ASpawn_VolumeForMonster>(FoundMonsterVolumes[0]);
			if (MonsterSpawnVolume)
			{
				MonsterSpawnVolume->SpawnRandomItem();
			}
		}
	}

	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&AAladinGameState:: OnLevelTimeUp,
		LevelDuration,
		false
	);
	GetWorldTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&AAladinGameState::SpawnItems,
		SpawnCycle,
		true
	);
}
void AAladinGameState::SpawnFallingHands()
{
	TArray<AActor*> FoundFallingHandsVolumes;
	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		ASpawn_VolumeFallingHands::StaticClass(),
		FoundFallingHandsVolumes
	);

	if (FoundFallingHandsVolumes.Num() > 0)
	{
		ASpawn_VolumeFallingHands* FallingHandsSpawnVolume =
			Cast<ASpawn_VolumeFallingHands>(FoundFallingHandsVolumes[0]);
		for (int32 i = 0; i < ItemToSpawn2; i++)
		{
			if (FallingHandsSpawnVolume)
			{
				FallingHandsSpawnVolume->SpawnRandomItem();
			}
		}
	
	}
}

void AAladinGameState::SpawnItems()
{
	
	WaveCount++;

	UE_LOG(LogTemp, Warning, TEXT("Wave %d Start!"), WaveCount);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Green,
			FString::Printf(TEXT("Wave %d Start!"), WaveCount)
		);
	}
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		ASpawn_Volume::StaticClass(),
		FoundVolumes
	);
	TArray<AActor*> FoundMonsterVolumes;

	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		ASpawn_VolumeForMonster::StaticClass(),
		FoundMonsterVolumes
	);
	ItemToSpawn *= 2;
	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawn_Volume* SpawnVolume = Cast<ASpawn_Volume>(FoundVolumes[0]);

			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();

				if (SpawnedActor && SpawnedActor->IsA(ACoin_Item::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
		if (FoundMonsterVolumes.Num() > 0)
		{
			ASpawn_VolumeForMonster* MonsterSpawnVolume =
				Cast<ASpawn_VolumeForMonster>(FoundMonsterVolumes[0]);

			if (MonsterSpawnVolume)
			{
				MonsterSpawnVolume->SpawnRandomItem();
			}
		}
	}
}
void AAladinGameState::OnLevelTimeUp()
{
	EndLevel();
}

void AAladinGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"), 
		CollectedCoinCount,
		SpawnedCoinCount); 
	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndLevel();
	}
}

void AAladinGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UAladinGameInstance* AladinGameInstance = Cast<UAladinGameInstance>(GameInstance);
		if (AladinGameInstance)
		{
			AddScore(Score);
			CurrentLevelIndex++;
			AladinGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}
	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();
		return;
	}
	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}	
}

void AAladinGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (APlayableAladinController* AladinPlayerController = Cast<APlayableAladinController>(PlayerController))
		{
			AladinPlayerController->SetPause(true);
			AladinPlayerController->ShowMainMenu(true);
		}
	}

}

void AAladinGameState::UpdateHUD()
{	//플레이어 컨트롤러 가져오기
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		//플레이어 컨트롤러를 APlayableAladinController로 캐스팅
		if (APlayableAladinController* AladinPlayerController = Cast<APlayableAladinController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = AladinPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}
				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UAladinGameInstance* AladinGameInstance = Cast<UAladinGameInstance>(GameInstance);
						
						if (AladinGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"),AladinGameInstance->TotalScore)));
						}
					}
				}
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex+1)));
				}
				if (UTextBlock* DebuffStackText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("DebuffStack"))))
				{
					if (APlayableAladin* PlayerCharacter =
						Cast<APlayableAladin>(UGameplayStatics::GetPlayerPawn(this, 0)))
					{
						DebuffStackText->SetText(
							FText::FromString(
								FString::Printf(
									TEXT("DebuffStack: %d"),
									PlayerCharacter->GetDebuffStack()
								)
							)
						);
					}
				}
			
			}
		}
	}
}