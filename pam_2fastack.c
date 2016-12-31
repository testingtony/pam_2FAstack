#define PAM_SM_ACCOUNT
#define PAM_SM_AUTH
#define PAM_SM_PASSWORD
#define PAM_SM_SESSION

#ifdef HAVE_SECURITY_PAM_APPL_H
#  include <security/pam_appl.h>
#endif
#ifdef HAVE_SECURITY_PAM_MODULES_H
#  include <security/pam_modules.h>
#endif

struct config {
  int echo;
  char *prompt;
};

#define NULL (char *)0;

int get_and_save_token(pam_handle_t *pamh, int flags, int argc, const char **argv);
int get_pass(pam_handle_t *pamh, char **token, struct config *config);
void parse_config(int argc, const char **argv, struct config *config);

int pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv) {
  return(PAM_IGNORE);
}

int pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv) {
  return(PAM_IGNORE);
}

int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv) {
  return(PAM_IGNORE);
}

int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
  return(get_and_save_token(pamh, flags, argc, argv));
}

int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
  return(PAM_IGNORE);
}

int pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc, const char **argv) {
  return(PAM_IGNORE);
}

int get_and_save_token(pam_handle_t *pamh, int flags, int argc, const char **argv) {

  char *pw = NULL;
  int rc = PAM_SESSION_ERR;
  struct config cfg;
  parse_config(argc, argv, &cfg);
  
  rc = get_pass(pamh, &pw, &cfg);

  if (rc == PAM_SUCCESS && pw) {
    rc = pam_set_item(pamh, PAM_AUTHTOK, pw);
  }
  
  return(rc);
}

#define NMSG 1
int get_pass(pam_handle_t *pamh, char **token, struct config *cfg) {
  struct pam_conv *conv;
  const struct pam_message *pmsg[NMSG];
  struct pam_message msg;
  struct pam_response *resp;
  int rc;

  rc = pam_get_item(pamh, PAM_CONV, (const void **) &conv);
  if (rc != PAM_SUCCESS) {
    return rc;
  }
  
  msg.msg = cfg->prompt ? cfg->prompt : "OTP: ";
  msg.msg_style = cfg->echo ? PAM_PROMPT_ECHO_ON : PAM_PROMPT_ECHO_OFF;
  pmsg[0] = &msg;

  rc = conv->conv(NMSG, pmsg, &resp, conv->appdata_ptr);

  if (rc == PAM_SUCCESS) {
    *token = resp->resp;
  }
  
  return rc;
}

void parse_config(int argc, const char **argv, struct config *cfg) {
  int i;

  memset(cfg, 0, sizeof(struct config));
  for (i = 0; i < argc; i++) {
    if (strcmp(argv[i], "echo") == 0)
      cfg->echo = 1;
    if (strncmp(argv[i], "prompt=", 7) == 0)
      cfg->prompt = argv[i] + 7;
  }
}
