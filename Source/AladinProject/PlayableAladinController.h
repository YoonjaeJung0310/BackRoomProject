// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayableAladinController.generated.h"
class UInputMappingContext;//IMC 관련 전방 선언
class UInputAction;//IA 관련 전방 선언
/**
 * 
 */
UCLASS()
class ALADINPROJECT_API APlayableAladinController : public APlayerController
{
	GENERATED_BODY()
public :
	APlayableAladinController();//생성자

	//에디터에서 세팅할 IMC
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	//IA_Move를 지정할 포인터 변수 선언
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	//IA_Look를 지정할 포인터 변수 선언
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	//IA_Jump를 지정할 포인터 변수 선언
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;
	//IA_Sprint를 지정할 포인터 변수 선언
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UUserWidget* HUDWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Menu")
	UUserWidget* MainMenuWidgetInstance;


	UFUNCTION(BlueprintPure,Category="HUD")
	UUserWidget* GetHUDWidget() const;
	
	UFUNCTION(BlueprintCallable,Category="HUD")
	void ShowGameHUD();
	UFUNCTION(BlueprintCallable, Category="Menu")
	void ShowMainMenu(bool bIsRestart);
	UFUNCTION(BlueprintCallable, Category="Menu")
	void StartGame();

	virtual void BeginPlay() override;


	
};
