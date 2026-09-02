// Fill out your copyright notice in the Description page of Project Settings.


#include "AladinGameMode.h"
#include "PlayableAladin.h"
#include "PlayableAladinController.h"

AAladinGameMode::AAladinGameMode()
{
	DefaultPawnClass = APlayableAladin::StaticClass();//기본 폰을 AYoonjae로 설정
	PlayerControllerClass = APlayableAladinController::StaticClass();//기본 플레이어 컨트롤러를 AYoonjaePlayerController로 설정
	GameStateClass = AAladinGameMode::StaticClass();
}
