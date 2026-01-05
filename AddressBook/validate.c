//include necessary header files
#include <stdio.h>
#include <string.h>
//include custom header file
#include "validate.h"
//function definitions
int validatename(char name[])//fun defination for name
{
    for(int i=0;name[i]!='\0';i++)//traverse till null character
    {
        if((name[i]==' ')||(name[i]=='.'))//condition for space and dot
        continue;//skip the next itteration
        if((name[i]>='a' && name[i]<='z')||(name[i]>='A' && name[i]<='Z'))//condition for alphabets
        continue;//skip the next itteration
        return 0;//0 means pass
    }
    return 1;//1 means fail
}
int validateno(char phone[])//fun defination for phone number
{
    int l = strlen(phone);//length of phone number
    if(l!=10) //condition for length
    return 0;//false
    for(int i=0;phone[i]!='\0';i++)//traverse till null character
    {
        if(phone[i]>'9' || phone[i]<'0')//condition for digits
        return 0;//false
    } 
    return 1;//1 means true. It means conditions are satisfied 
}
int validatmail(char mail[])//fun defination for mail.id
{
    int len = strlen(mail);//length of mail.id
    int j = 0;//iterator for loop
    if((mail[0] >='0' && mail[0] <='9') || len < 11 )//condition for first character and length
    return 0;//false
    char rev[len+1];//string to hold reverse string
    for(int i=0;i<len;i++)//string reverse loop
        rev[i]=mail[len-i-1];//String reverse
    rev[len] = '\0';//null character at end
    char str2[]="moc.liamg@";//string to compare
    for(int i=0;str2[i]!='\0';i++)//traverse till null character
    {
        if(str2[i]==rev[i]) //condition to check gmail.com in reverse
            continue;//skip the next itteration
        return 0;//false
    }
    while (j < len-10)//len-11 means @gmail.com was not checking
    {
        if((mail[j] >='a' && mail[j] <= 'z') ||//only lower case
            (mail[j] >= '0' && mail[j] <= '9') ||//check digits
           mail[j] == '.' || mail[j] == '_')//check dot or underscore
        {
            j++;
            continue;//its true goto next itteration
        }
        else
            return 0;//false
    }
    return 1;//true
} 
int checkDuplicatename(char name[],AddressBook *addressBook)//fun defination to check duplicate name
{
    //traverse through address book contacts
    for(int i=0;i<addressBook->contactCount;i++)//loop through existing contacts
    {
        //compare input name with existing contact names
        if(strcmp(name,addressBook->contacts[i].name)==0)
        return 0;//duplicate found
    }
    return 1;//no duplicate
}
int checkDuplicateno(char phone[],AddressBook *addressBook)//fun defination to check duplicate phone number
{
    //traverse through address book contacts
    for(int i=0;i<addressBook->contactCount;i++)//loop through existing contacts
    {
        //compare input phone with existing contact phones
        if(strcmp(phone,addressBook->contacts[i].phone)==0)
        return 0;//duplicate found
    }
    return 1;//no duplicate
}
int checkDuplicatmail(char mail[],AddressBook *addressBook)//fun defination to check duplicate mail id
{
    //traverse through address book contacts
    for(int i=0;i<addressBook->contactCount;i++)//loop through existing contacts
    {
        //compare input mail with existing contact mails
        if(strcmp(mail,addressBook->contacts[i].mail)==0)
        return 0;//duplicate found
    }
    return 1;//no duplicate
}