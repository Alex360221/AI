// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

Item::Item()
{
}

Item::Item(int id, FString name, int amount, float weight, int type)
{
	item_ID = id;
	item_Name = name;
	item_Amount = amount;
	item_Weight = weight;
	item_Type = type;
}

