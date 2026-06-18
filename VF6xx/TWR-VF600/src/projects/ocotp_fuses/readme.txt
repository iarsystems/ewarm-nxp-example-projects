FUSE CHANGES ARE PERMANENT. ENSURE YOU ARE BLOWING THE CORRECT FUSES  


  /* Fuse location and descriptions are in Chapter 36 in the 
   * OCOTP_CFGx registers 
   *
   * Fuses are 0 by default. Once blown to '1', the change is permanent.
   *  Be careful to ensure you are blowing the correct fuses. Ensure 
   *  other fuses have not already been blown that would interfere with fuses
   *  you want to blow. 
   *
   * The code below will set the fuses to boot from SD Card
   *
   * The shadow registers where fuse state can be viewed starts at 0x400a5400
   */