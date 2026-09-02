// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"//캐릭터가 기본적으로 동작하기 위해 필수적인 기능들이 들어가 있는 헤더파일 추가
#include "GameFramework/Character.h"//게임 구조 중에서 캐릭터를 위한 구조 헤더파일 추가
#include "Logging/LogMacros.h"
#include "Blueprint/UserWidget.h"
#include "PlayableAladin.generated.h"//알라딘용 UHT 연결 Generated 함수 헤더파일 추가

class USpringArmComponent;//스프링 암 관련 클래스 헤더 선언
class UCameraComponent;//카메라 관련 클래스 전방 선언
class UWidgetComponent;//위젯 전방 선언
struct FInputActionValue;//Enhanced Input에서 액션 값을 받을 구조체 전방 선언


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class ALADINPROJECT_API APlayableAladin : public ACharacter
{
	GENERATED_BODY()//연결 매크로

public:
	// Sets default values for this character's properties
	APlayableAladin();//생성자
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	UWidgetComponent* OverheadWidget;

	void UpdateOverheadHP();
	float GetSpeed();
	void SetSpeed(float normalspeed);
	float GetDisableMoveTime() const;
	void SetDisableMoveTime(float Time);
	int32 GetDebuffStack() const;
	void AddDebuffStack();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;//게임 시작시
	//스프링 암 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;//USpringArmComponent 객체를 가리키는 포인터 SpringArmComp 생성
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;//UCameraComponent 객체를 가리키는 포인터 CameraComp 생성
	//이동 속도 관련 매개변수들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed;//기본 걷기 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	int32 DebuffStack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	float DisableMoveTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;//달리기 속도를 몇배로 할거냐
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed;//실제 달리기 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 입력 바인딩을 처리할  가상 함수
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//IA_MOVE와 IA_Jump 등을 처리할 함수 원형
	//Enhanced Input에서 액션 값을 FInPutActionValue로 전달됩니다(F은 Fucntion의 약자입니당)
	UFUNCTION()
	void Move(const FInputActionValue& value);//입력값을 담은 FInputActionValue 객체를 참조
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	void OnDeath();
	virtual float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent, //어떤 데미지 이벤트인지 (불,얼음 등)
		AController* EventInstigator,//누가 데미지를 입혔는지
		AActor* DamageCauser)//데미지를 일으킨 오브젝트
		override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> VideoWidgetClass;


	UPROPERTY()
	UUserWidget* VideoWidgetInstance;
};
