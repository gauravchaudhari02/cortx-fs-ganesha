#ifndef KVSFS_METHODS_H_
#define KVSFS_METHODS_H_

#include <fsal_api.h>
#include <cortxfs.h>
#include <cortxfs_fh.h>
#include <gsh_list.h>
#include <fsal_types.h>


/******************************************************************************/
struct kvsfs_fsal_index_context;

/* Wrapper for a File Handle object. */
struct kvsfs_file_handle {
	uint64_t fs_id;
	cfs_ino_t kvsfs_handle;
};

/* ds_wire handle. */
struct kvsfs_ds_wire {
	uint64_t fsid;
	cfs_ino_t ino;
};
struct kvsfs_fsal_export {
	/* base */
	struct fsal_export export;

	/** An CORTXFS filesystem in Open state */
	struct cfs_fs *cfs_fs;

	/* TODO: This field should be a property of a FS object */
	uint64_t fs_id;

	/** Export config. */
	char *cfs_config;

	/* PNFS, MDS and DS need be enabled on FSAL export */	
	bool pnfs_ds_enabled;
	bool pnfs_mds_enabled;
};

/** Get export's Root handle by path. */
fsal_status_t kvsfs_lookup_path(struct fsal_export *exp_hdl,
			       const char *path,
			       struct fsal_obj_handle **handle,
			       struct attrlist *attrs_out);

/**Open a filesystem and get its descriptor. */
int kvsfs_export_to_cortxfs_ctx(struct fsal_export *exp_hdl,
		 	        struct cfs_fs **cfs_fs);

/** Create a file handle within the expecified NFS export. */
fsal_status_t kvsfs_create_handle(struct fsal_export *exp_hdl,
				 struct gsh_buffdesc *hdl_desc,
				 struct fsal_obj_handle **handle,
				 struct attrlist *attrs_out);

struct state_t *kvsfs_alloc_state(struct fsal_export *exp_hdl,
				enum state_type state_type,
				struct state_t *related_state);

void kvsfs_free_state(struct fsal_export *exp_hdl, struct state_t *state);
fsal_status_t kvsfs_extract_handle(struct fsal_export *exp_hdl,
					 fsal_digesttype_t in_type,
					 struct gsh_buffdesc *fh_desc,
					 int flags);

/******************************************************************************/

#if 0
/* KVSFS methods for handles
 */

void kvsfs_handle_ops_init(struct fsal_obj_ops *ops);

/* method proto linkage to handle.c for export
 */

fsal_status_t kvsfs_lookup_path(struct fsal_export *exp_hdl,
			       const char *path,
			       struct fsal_obj_handle **handle);

/*
 * KVSFS internal export
 */

/* Helper Methods */
int kvsfs_get_fsid(const struct fsal_obj_handle *hdl, cfs_fsid_t *fs_id);
int kvsfs_obj_to_cfs_ctx(const struct fsal_obj_handle *hdl, cfs_ctx_t *fs_ctx);

	/* I/O management */
fsal_status_t kvsfs_open(struct fsal_obj_handle *obj_hdl,
			fsal_openflags_t openflags);
fsal_status_t kvsfs_commit(struct fsal_obj_handle *obj_hdl,	/* sync */
			  off_t offset, size_t len);
fsal_openflags_t kvsfs_status(struct fsal_obj_handle *obj_hdl);
fsal_status_t kvsfs_read(struct fsal_obj_handle *obj_hdl,
			uint64_t offset,
			size_t buffer_size, void *buffer, size_t *read_amount,
			bool *end_of_file);
fsal_status_t kvsfs_write(struct fsal_obj_handle *obj_hdl,
			 uint64_t offset,
			 size_t buffer_size, void *buffer,
			 size_t *write_amount, bool *fsal_stable);
fsal_status_t kvsfs_share_op(struct fsal_obj_handle *obj_hdl, void *p_owner,
			    fsal_share_param_t request_share);
fsal_status_t kvsfs_close(struct fsal_obj_handle *obj_hdl);
fsal_status_t kvsfs_lru_cleanup(struct fsal_obj_handle *obj_hdl,
			       lru_actions_t requests);

/* extended attributes management */
fsal_status_t kvsfs_list_ext_attrs(struct fsal_obj_handle *obj_hdl,
				  unsigned int cookie,
				  fsal_xattrent_t *xattrs_tab,
				  unsigned int xattrs_tabsize,
				  unsigned int *p_nb_returned,
				  int *end_of_list);
fsal_status_t kvsfs_getextattr_id_by_name(struct fsal_obj_handle *obj_hdl,
					 const char *xattr_name,
					 unsigned int *pxattr_id);
fsal_status_t kvsfs_getextattr_value_by_name(struct fsal_obj_handle *obj_hdl,
					    const char *xattr_name,
					    caddr_t buffer_addr,
					    size_t buffer_size,
					    size_t *p_output_size);
fsal_status_t kvsfs_getextattr_value_by_id(struct fsal_obj_handle *obj_hdl,
					  unsigned int xattr_id,
					  caddr_t buffer_addr,
					  size_t buffer_size,
					  size_t *p_output_size);
fsal_status_t kvsfs_setextattr_value(struct fsal_obj_handle *obj_hdl,
				    const char *xattr_name, caddr_t buffer_addr,
				    size_t buffer_size, int create);
fsal_status_t kvsfs_setextattr_value_by_id(struct fsal_obj_handle *obj_hdl,
					  unsigned int xattr_id,
					  caddr_t buffer_addr,
					  size_t buffer_size);
fsal_status_t kvsfs_getextattr_attrs(struct fsal_obj_handle *obj_hdl,
				    unsigned int xattr_id,
				    struct attrlist *p_attrs);
fsal_status_t kvsfs_remove_extattr_by_id(struct fsal_obj_handle *obj_hdl,
					unsigned int xattr_id);
fsal_status_t kvsfs_remove_extattr_by_name(struct fsal_obj_handle *obj_hdl,
					  const char *xattr_name);
fsal_status_t kvsfs_lock_op(struct fsal_obj_handle *obj_hdl,
			   void *p_owner,
			   fsal_lock_op_t lock_op,
			   fsal_lock_param_t *request_lock,
			   fsal_lock_param_t *conflicting_lock);

/* external storage */
int external_write(struct fsal_obj_handle *obj_hdl,
		   uint64_t offset,
		   size_t buffer_size,
		   void *buffer,
		   size_t *write_amount,
		   bool *fsal_stable,
		   struct stat *stat);

int external_read(struct fsal_obj_handle *obj_hdl,
		  uint64_t offset,
		  size_t buffer_size,
		  void *buffer,
		  size_t *read_amount,
		  bool *end_of_file);

int external_consolidate_attrs(struct fsal_obj_handle *obj_hdl,
			       struct stat *kvsstat);

int external_unlink(struct fsal_obj_handle *dir_hdl,
		    const char *name);

int external_truncate(struct fsal_obj_handle *obj_hdl,
		      off_t filesize);

#endif // 0
#endif /* KVSFS_METHODS_H_ */
