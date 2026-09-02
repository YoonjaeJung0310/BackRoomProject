#include "Healing_Item.h"
#include "PlayableAladin.h"

AHealing_Item::AHealing_Item()
{
	HealAmount = 20;
	ItemType = "HealingItem";
}

void AHealing_Item::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (APlayableAladin* PlayerCharacter = Cast<APlayableAladin>(Activator))
		{
			PlayerCharacter->AddHealth(HealAmount);
		}
		DestroyItem();
	}
}