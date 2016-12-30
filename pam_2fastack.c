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

#define NULL (char *)0;

int get_and_save_token(pam_handle_t *pamh, int flags, int argc, const char **argv);
char *get_pass(pam_handle_t *pamh);

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
  
  pw = get_pass(pamh);

  if (pw && pam_set_item(pamh, PAM_AUTHTOK, pw) == PAM_SUCCESS) {
    rc = PAM_SUCCESS;
  }
  
  return(rc);
}

#define NMSG 1
char *get_pass(pam_handle_t *pamh) {
  struct pam_conv *conv;
  const struct pam_message *pmsg[NMSG];
  struct pam_message msg;
  struct pam_response *resp;
  int rc;

  rc = pam_get_item(pamh, PAM_CONV, (const void **) &conv);
  if (rc != PAM_SUCCESS) {
    return NULL;
  }
  
  msg.msg = "OTP: ";
  msg.msg_style = PAM_PROMPT_ECHO_OFF;
  pmsg[0] = &msg;

  /*  resp = ; */

  rc = conv->conv(NMSG, pmsg, &resp, conv->appdata_ptr);

  return resp->resp;
}
