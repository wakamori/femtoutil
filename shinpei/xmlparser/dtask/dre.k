"""
  Wrapper for DRE 
"""


/* ************************************************************************ */
/*     Application Container                                                */
/* ************************************************************************ */
class AppContainer {
	String name;
	String config_data;

}

// config_data: <technology>:<parameters for technology>
//              lxc:port
AppContainer AppContainer.new (String name, String config_data, AppContainer _);

DRE_APP_CONTAINER_STATUS_STOP = 1;
DRE_APP_CONTAINER_STATUS_RUNNIG = 2;
DRE_APP_CONTAINER_STATUS_SUSPENDED = 3;


// Destroy System Container in the underlying OS
// This is NOT the destructor of Konoha instance 
int AppContainer.destroy (void);

AppContainer AppContainer.open (String name, AppContainer _);
int AppContainer.start (void);
int AppContainer.stop (void);
int AppContainer.close (void);
int AppContainer.status (void);

int AppContainer.suspend (void);
int AppContainer.resume (void);
int AppContainer.checkpoint (String file);
int AppContainer.restart (String file);

int AppContainer.list (Array <String> found_names); 

int AppContainer.resource_get (int resource);
int AppContainer.resource_set (int resource, int val);

int AppContainer.snapshot_list (Array <String> list);
int AppContainer.snapshot_save (String snapshot_name);
int AppContainer.snapshot_load (String snapshot_name);
int AppContainer.snapshot_remove (String snapshot_name);
int AppContainer.snapshot_commit (String snapshot_name, String base_name);

/* ***************************************************************** */
/*   System Container                                                */
/* ***************************************************************** */

class SystemContainer {
	String name;
	String config_data;
}

SystemContainer SystemContainer.new (String name, String config_data, SystemContainer _);

// This is NOT the destructor of Konoha instance 
int SystemContainer.destroy (void);


int SystemContainer.open (void);
int SystemContainer.start (void);
int SystemContainer.stop (void);
int SystemContainer.close (void);
int SystemContainer.status (void);


int SystemContainer.suspend (void);
int SystemContainer.resume (void);
int SystemContainer.checkpoint (String file);
int SystemContainer.restart (String file);

int SystemContainer.list (Array <String> found_names); 

int SystemContainer.resource_get (int resource);
int SystemContainer.resource_set (int resource, int val);


SystemContainer SystemContainer.migration_target_start (String name, String config_data);
int SystemContainer.migration_start (String target);


int SystemContainer.snapshot_list (Array <String> list);
int SystemContainer.snapshot_save (String snapshot_name);
int SystemContainer.snapshot_load (String snapshot_name);
int SystemContainer.snapshot_remove (String snapshot_name);
int SystemContainer.snapshot_commit (String snapshot_name, String base_name);

