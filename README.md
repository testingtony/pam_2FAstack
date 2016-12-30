pam_2FAstack
============

An attempt to pass the same token to multiple otp modules

Compiling and Installing
------------------------

As  normal user

```
$ autoconf
$ ./configure
$ make
$ sudo make install
```

as root

```
# ls -s /usr/local/lib/security/pam_2fastack.so /lib/security/
```

Configure for separate system password and otp prompts
------------------------------------------------------

/etc/pam.d/sshd
```
auth required   pam_2fastack.so
auth [success=2 default=ignore] pam_google_authenticator.so use_first_pass forward_pass
auth [success=1 default=ignore] pam_yubico.so id=#### use_first_pass
auth required   pam_deny.so
auth required   pam_unix.so nullok_secure use_first_pass
```

Configure for combined system password and otp prompt
-----------------------------------------------------

/etc/pam.d/sshd
```
auth required   pam_unix.so nullok_secure
auth required   pam_2fastack.so
auth [success=2 default=ignore] pam_google_authenticator.so use_first_pass
auth [success=1 default=ignore] pam_yubico.so id=#### use_first_pass
auth required   pam_deny.so
```

pam2fastack.so arguments
------------------------

### prompt=xxx

Define the OTP prompt text. Put in square brackets for spaces e.g. `pam_2fastack.so [prompt=what you have: ]`

### echo

Causes the otp to be echoed back when typed.



Source
------

From [Roy Keene's blog][KBLOG] on creating pam modules

---

>                               pam_success 0.1
>
>Release information:
>  pkg:  pam_success version 0.1
>  date: Tue Oct 25 00:40:09 GMT 2011
>

[KBLOG]: http://www.rkeene.org/projects/info/wiki/222
