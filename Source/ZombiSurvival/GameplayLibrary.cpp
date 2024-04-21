#include "GameplayLibrary.h"

bool UGameplayLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	return !bBothAreEnemies;
}