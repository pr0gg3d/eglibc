/* Copyright (C) 1998,2000,2003,2004,2006 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <errno.h>
#include <unistd.h>
#include <setxid.h>
#include <linux/posix_types.h>
#include <kernel-features.h>


#ifdef __NR_setregid32
# if __ASSUME_32BITUIDS == 0
/* This variable is shared with all files that need to check for 32bit
   uids.  */
extern int __libc_missing_32bit_uids;
# endif
#endif /* __NR_setregid32 */

int
__setregid (gid_t rgid, gid_t egid)
{
  int result;

#if __ASSUME_32BITUIDS > 0
  result = INLINE_SETXID_SYSCALL (setregid32, 2, rgid, egid);
#else
# ifdef __NR_setregid32
  if (__libc_missing_32bit_uids <= 0)
    {
      int saved_errno = errno;

      result = INLINE_SETXID_SYSCALL (setregid32, 2, rgid, egid);

      if (result == 0)
	goto out;
      if (errno != ENOSYS)
	return result;

      __set_errno (saved_errno);
      __libc_missing_32bit_uids = 1;
    }
# endif /* __NR_setregid32 */
  if (((rgid + 1) > (gid_t) ((__kernel_gid_t) -1U))
      || ((egid + 1) > (gid_t) ((__kernel_gid_t) -1U)))
    {
      __set_errno (EINVAL);
      return -1;
    }

  result = INLINE_SETXID_SYSCALL (setregid, 2, rgid, egid);
# ifdef __NR_setregid32
 out:
# endif
#endif

  return result;
}
#ifndef __setregid
weak_alias (__setregid, setregid)
#endif
