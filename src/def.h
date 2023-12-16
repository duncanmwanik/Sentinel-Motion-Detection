// Insert Firebase project API Key
#define API_KEY "API_KEY"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "USER_EMAIL"
#define USER_PASSWORD "USER_PASSWORD"

// Insert Firebase storage bucket ID e.g bucket-name.appspot.com
#define STORAGE_BUCKET_ID "STORAGE_BUCKET_ID"

// Photo File Name to save in SPIFFS
#define FILE_PHOTO "/data/photo.jpg"

#define FLASH_GPIO_NUM 4

#define MAX_WIFI_RECCONECTS 5

// Post Image To Firebase
void setupFirebase();
void postImage();

// Motion Detection
void setupMotion();
void detectMotion();

// Get current time
void setupTime();
String getFileName();