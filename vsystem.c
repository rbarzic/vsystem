#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include  <vpi_user.h> /* IEEE 1364 PLI VPI routine library */

int vsystem_compiletf(__attribute__((unused)) PLI_BYTE8 *user_data)
{
  vpi_printf("-D- vsystem_compiletf called\n");

    vpiHandle systf_handle, arg_iterator, arg_handle;
    PLI_INT32 tfarg_type;
    int err_flag = 0;
    /* obtain a handle to the system task instance */
    systf_handle = vpi_handle(vpiSysTfCall, NULL);
    /* obtain handles to system task arguments */
    arg_iterator = vpi_iterate(vpiArgument, systf_handle);
    if (arg_iterator == NULL) {
      vpi_printf("ERROR: $vsystem requires 1 argument\n");
      err_flag = 1;
    }
    else {
      /* check the type of object in system task arguments */
      arg_handle = vpi_scan(arg_iterator);
      tfarg_type = vpi_get(vpiType, arg_handle);
      if (tfarg_type != vpiConstant) {
        vpi_printf("ERROR: $show_all_nets arg must be module instance\n");
        vpi_free_object(arg_iterator); /* free iterator memory */
        err_flag = 1;
      }
    
  
    else {
      /* check that there is only 1 system task argument */
      arg_handle = vpi_scan(arg_iterator);
      if (arg_handle != NULL) {
        vpi_printf("ERROR: $show_all_nets can only have 1 argument\n");
        vpi_free_object(arg_iterator); /* free iterator memory */
        err_flag = 1;
      }
    }
  }

  if (err_flag) {
     vpi_control(vpiFinish, 1); /* abort simulation */
  }
  return(0);

}




int vsystem_calltf(__attribute__((unused)) PLI_BYTE8* user_data)
{
  vpi_printf("-D- vsystem_calltf called\n");

    vpiHandle    systf_h, arg_itr, cmd_string_h;
    s_vpi_value argVal;
    s_vpi_error_info err;  /* structure for error handling */
    s_vpi_value value_s;
    
    
    /* obtain a handle to the system task instance */
    systf_h = vpi_handle(vpiSysTfCall, NULL);
  
    /* obtain handle to system task argument */
    /* compiletf has already verified only 1 arg with correct type */
    arg_itr = vpi_iterate(vpiArgument, systf_h);
    cmd_string_h = vpi_scan(arg_itr);
    vpi_free_object(arg_itr);  /* free iterator--did not scan to null */


    argVal.format = vpiStringVal;
    vpi_get_value(cmd_string_h, &argVal);
    if (vpi_chk_error(&err)) {
      vpi_printf("ERROR: PLIbook_get_arg_string_val_vpi() could not obtain arg value\n");
      vpi_printf("File %s, Line %d: %s\n", err.file, err.line,
                  err.message);
    }
    vpi_printf("-D- Command %s\n", argVal.value.str);
    uint32_t result = system(argVal.value.str);
    value_s.format = vpiIntVal;
    value_s.value.integer = result;
    vpi_printf("-D- Result %d\n", result);
    vpi_put_value(systf_h, &value_s, NULL, vpiNoDelay);
    return 0;
}


PLI_INT32 vsystem_sizetf(__attribute__((unused)) PLI_BYTE8 *user_data)
{
return(32); /* vsystem returns 32-bit values */
}

void vsystem_register(void)
{
      s_vpi_systf_data tf_data;

      tf_data.type      = vpiSysFunc;
      tf_data.sysfunctype = vpiSysFuncSized;
      tf_data.tfname    = "$vsystem";
      tf_data.calltf    = vsystem_calltf;
      tf_data.compiletf = vsystem_compiletf;
      tf_data.sizetf    = vsystem_sizetf;
      tf_data.user_data = 0;
      vpi_register_systf(&tf_data);
      return;
}

#ifndef VSYSTEM_NO_VLOG_STARTUP_ROUTINES

void (*vlog_startup_routines[])(void) = {
    vsystem_register,
    0
};
#endif
