#include "DisguiseHealingItem.h"
#include "PlayableAladin.h"
#include "PlayableAladinController.h"

ADisguiseHealingItem::ADisguiseHealingItem()
{
    DebuffPercent = 0.8f;
    DisableMoveDuration = 2.0f;

    ItemType = "HealingItem";
}

void ADisguiseHealingItem::ActivateItem(AActor* Activator)
{
    Super::ActivateItem(Activator);

    if (Activator && Activator->ActorHasTag("Player"))
    {
        if (APlayableAladin* PlayerCharacter = Cast<APlayableAladin>(Activator))
        {
            PlayerCharacter->AddDebuffStack();
            PlayerCharacter->SetSpeed(
                PlayerCharacter->GetSpeed() * DebuffPercent
            );

            if (APlayableAladinController* PC =
                Cast<APlayableAladinController>(PlayerCharacter->GetController()))
            {
                PC->SetIgnoreMoveInput(true);

                FTimerHandle TimerHandle;
                GetWorld()->GetTimerManager().SetTimer(
                    TimerHandle,
                    [PC]()
                    {
                        if (PC)
                        {
                            PC->SetIgnoreMoveInput(false);
                        }
                    },
                   DisableMoveDuration,
                    false
                );
            }
         
            DestroyItem();
        }
    }
}