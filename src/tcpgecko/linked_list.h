#pragma once

struct node
{
	void* data;
	struct node* next;
};

void destroy(struct node* list);

struct node* insert(struct node* list, void* data);

int length(struct node* list);

void reverse(struct node** list);
