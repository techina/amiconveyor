#ifndef leadblow_LBSocial_h
#define leadblow_LBSocial_h

class LBSocial {
public:
    static void tweet(const char* message, const char* ssFilePath);
    static void facebook(const char* message, const char* ssFilePath);
};

#endif
