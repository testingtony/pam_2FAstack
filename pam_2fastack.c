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
int get_pass(pam_handle_t *pamh, char **token);

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
  
  rc = get_pass(pamh, &pw);

  if (rc == PAM_SUCCESS && pw) {
    rc = pam_set_item(pamh, PAM_AUTHTOK, pw);
  }
  
  return(rc);
}

#define NMSG 1
int get_pass(pam_handle_t *pamh, char **token) {
  struct pam_conv *conv;
  const struct pam_message *pmsg[NMSG];
  struct pam_message msg;
  struct pam_response *resp;
  int rc;

  rc = pam_get_item(pamh, PAM_CONV, (const void **) &conv);
  if (rc != PAM_SUCCESS) {
    return rc;
  }
  
  msg.msg = "OTP: ";
  msg.msg_style = PAM_PROMPT_ECHO_OFF;
  pmsg[0] = &msg;

  /*  resp = ; */

  rc = conv->conv(NMSG, pmsg, &resp, conv->appdata_ptr);

  if (rc == PAM_SUCCESS) {
    *token = resp->resp;
  }
  
  return rc;
}
