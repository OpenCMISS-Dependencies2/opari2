/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2009-2013,
 * RWTH Aachen University, Germany
 *
 * Copyright (c) 2009-2013,
 * Gesellschaft fuer numerische Simulation mbH Braunschweig, Germany
 *
 * Copyright (c) 2009-2013,
 * Technische Universitaet Dresden, Germany
 *
 * Copyright (c) 2009-2013,
 * University of Oregon, Eugene, USA
 *
 * Copyright (c) 2009-2013, 2019,
 * Forschungszentrum Juelich GmbH, Germany
 *
 * Copyright (c) 2009-2013,
 * German Research School for Simulation Sciences GmbH, Juelich/Aachen, Germany
 *
 * Copyright (c) 2009-2013,
 * Technische Universitaet Muenchen, Germany
 *
 * This software may be modified and distributed under the terms of
 * a BSD-style license. See the COPYING file in the package base
 * directory for details.
 *
 */
/****************************************************************************
**  SCALASCA    http://www.scalasca.org/                                   **
**  KOJAK       http://www.fz-juelich.de/jsc/kojak/                        **
*****************************************************************************
**  Copyright (c) 1998-2009                                                **
**  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
**                                                                         **
**  See the file COPYRIGHT in the package base directory for details       **
****************************************************************************/
#ifndef POMP2_LIB_H
#define POMP2_LIB_H

#ifdef _OPENMP
#include <omp.h>
#endif

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @file
 *
 *  @brief      This file contains the declarations of all
 *              POMP2 functions.
 *
 */

/* Avoid warnings from Orcale Studio Compiler if nowait clauses are added
 * to reduction loops. Opari always inserts a barrier after such loops, so
 * the nowait is  save.*/
#if defined( __SUNPRO_C )
#pragma error_messages (off, E_NOWAIT_REDUCTION_USE)
#endif

/** Handles to identify OpenMP regions. */

/** To avoid multiple typedefs of OPARI2_Region_handle*/
#ifndef __opari2_region_handle
typedef void* OPARI2_Region_handle;
#define __opari2_region_handle
#endif

typedef OPARI2_Region_handle POMP2_Region_handle;
typedef int64_t              POMP2_Task_handle;

/** @name Functions generated by the instrumenter */
/*@{*/
/**
 * Returns the number of instrumented regions.@n
 * The instrumenter scans all OPARI2-created include files with nm and greps
 * the POMP2_INIT_uuid_numRegions() function calls. Here we return the sum of
 * all numRegions.
 * @return number of instrumented regions
 */
extern size_t
POMP2_Get_num_regions( void );

/**
 * Init all OPARI2-created regions.@n
 * The instrumentor scans all OPARI2-created include files with nm and greps
 * the POMP2_INIT_uuid_numRegions() function calls. The instrumentor then
 * defines these functions by calling all grepped functions.
 */
extern void
POMP2_Init_regions( void );

/**
 * Returns the OPARI2 version.
 * @return version string
 */
extern const char*
POMP2_Get_opari2_version( void );

/*@}*/
/** Function that returns a new task handle.
 * @return new task handle */

extern POMP2_Task_handle
POMP2_Get_new_task_handle( void );

/** Create a unique mapping between @a ctc_string and the
    implementation-defined @a pomp2_handle. Be aware that
    POMP2_Assign_handle() is called from POMP2_Init_regions()
    in a serial context but might get called concurrently as well.
 */
extern void
POMP2_Assign_handle( POMP2_Region_handle* pomp2_handle,
                     const char           ctc_string[] );

#ifdef _OPENMP
/** Called before an atomic statement.

    @param pomp2_handle The handle of the started region.
    @param ctc_string   Initialization string. May be ignored if \<pomp2_handle\> is already initialized.
 */
extern void
POMP2_Atomic_enter( POMP2_Region_handle* pomp2_handle,
                    const char           ctc_string[] );

/** Called after an atomic statement.
    @param pomp2_handle  The handle of the ended region.
 */
extern void
POMP2_Atomic_exit( POMP2_Region_handle* pomp2_handle );

/** Called before a barrier. \n

    \e OpenMP \e 3.0: Barriers can be used as scheduling points for
    tasks. When entering a barrier the task id of the currently
    executing task (\e pomp2_current_task) is saved in \e
    pomp2_old_task, which is defined inside the instrumented user
    code.

    @param pomp2_handle   The handle of the started region.
    @param pomp2_old_task Pointer to a "taskprivate" variable where the current task id is stored.
    @param ctc_string     Initialization string. May be ignored if \<pomp2_handle\> is already initialized.
 */
extern void
POMP2_Barrier_enter( POMP2_Region_handle* pomp2_handle,
                     POMP2_Task_handle*   pomp2_old_task,
                     const char           ctc_string[] );

/** Called after a barrier. \n

    \e OpenMP \e 3.0: When a task exits a barrier the variable \e
    pomp2_old_task (defined in the instrumented user code) holds the
    id of the task that entered the barrier. The value is stored in
    the adapter (in \e pomp2_current_task) to be made available for
    the measurement system for the following regions.

    @param pomp2_handle   The handle of the ended region.
    @param pomp2_old_task "Taskprivate" variable storing the id of the task the barrier is located in.
 */
extern void
POMP2_Barrier_exit( POMP2_Region_handle* pomp2_handle,
                    POMP2_Task_handle    pomp2_old_task );


/** Called before an implicit barrier.

    \e OpenMP \e 3.0: Barriers can be used as scheduling points for
    tasks. When entering a barrier the task id of the currently
    executing task (\e pomp2_current_task) is saved in \e
    pomp2_old_task, which is defined inside the instrumented user
    code.

    @param pomp2_handle   The handle of the started region.
    @param pomp2_old_task Pointer to a "taskprivate" variable where the current task id is stored.
 */
extern void
POMP2_Implicit_barrier_enter( POMP2_Region_handle* pomp2_handle,
                              POMP2_Task_handle*   pomp2_old_task );


/** Called after an implicit barrier.

    \e OpenMP \e 3.0: When a task exits a barrier the variable \e
    pomp2_old_task (defined in the instrumented user code) holds the
    id of the task that entered the barrier. The value is stored in
    the adapter (in \e pomp2_current_task) to be made available for
    the measurement system for the following regions.

    @param pomp2_handle   The handle of the started region.
    @param pomp2_old_task "Taskprivate" variable storing the id the task the implicit barrier is used in.
 */
extern void
POMP2_Implicit_barrier_exit( POMP2_Region_handle* pomp2_handle,
                             POMP2_Task_handle    pomp2_old_task );


/** Called before an flush.

    @param pomp2_handle  The handle of the started region.
    @param ctc_string   Initialization string. May be ignored if \<pomp2_handle\> is already initialized.
 */
extern void
POMP2_Flush_enter( POMP2_Region_handle* pomp2_handle,
                   const char           ctc_string[] );

/** Called after an flush.
    @param pomp2_handle  The handle of the ended region.
 */
extern void
POMP2_Flush_exit( POMP2_Region_handle* pomp2_handle );

/** Called at the start of a critical region.
    @param pomp2_handle  The handle of the started region.
 */
extern void
POMP2_Critical_begin( POMP2_Region_handle* pomp2_handle );

/** Called at the end of a critical region.
    @param pomp2_handle  The handle of the ended region.
 */
extern void
POMP2_Critical_end( POMP2_Region_handle* pomp2_handle );

/** Called before a critical region.
    @param pomp2_handle  The handle of the started region.
    @param ctc_string   Initialization string. May be ignored if \<pomp2_handle\> is already initialized.
 */
extern void
POMP2_Critical_enter( POMP2_Region_handle* pomp2_handle,
                      const char           ctc_string[] );

/** Called after a critical region.
    @param pomp2_handle  The handle of the region.
 */
extern void
POMP2_Critical_exit( POMP2_Region_handle* pomp2_handle );

/** Called before a for loop.
    @param pomp2_handle  The handle of the region.
    @param ctc_string   Initialization string. May be ignored if \<pomp2_handle\> is already initialized.
 */
extern void
POMP2_For_enter( POMP2_Region_handle* pomp2_handle,
                 const char           ctc_string[] );

/** Called after a for loop.
    @param pomp2_handle  The handle of the region.
 */
extern void
POMP2_For_exit( POMP2_Region_handle* pomp2_handle );

/** Called at the start of a master region.
    @param pomp2_handle  The handle of the region.
    @param ctc_string   Initialization string. May be ignored if \<pomp2_handle\> is already initialized.
 */
extern void
POMP2_Master_begin( POMP2_Region_handle* pomp2_handle,
                    const char           ctc_string[] );

/** Called at the end of a master region.
    @param pomp2_handle  The handle of the ended region.
 */
extern void
POMP2_Master_end( POMP2_Region_handle* pomp2_handle );

/** Called at the start of a parallel region.

    \e OpenMP \e 3.0: When a new parallel region is entered, each
    thread taking part in that region is executed as a task. These
    tasks are assigned a new unique task id which is stored in \e
    pomp2_current_task.

    @param pomp2_handle  The handle of the region.
 */
extern void
POMP2_Parallel_begin( POMP2_Region_handle* pomp2_handle );

/** Called at the end of a parallel region.

    @param pomp2_handle The handle of the region.
 */
extern void
POMP2_Parallel_end( POMP2_Region_handle* pomp2_handle );

/** Called before a parallel region.

    \e OpenMP \e 3.0: The task id of the currently executing task (\e
    pomp2_current_task) is saved in \e pomp2_old_task, which is
    defined inside the instrumented user code. In each of the threads
    taking part in the following parallel region a newly defined
    unique task id is assigned (see \ref POMP2_Parallel_begin)

    @param pomp2_handle   The handle of the region.
    @param if_clause      Value of the argument of an if clause (if present).
    @param num_threads    Upper bound for number of child threads.
    @param pomp2_old_task The task id of the region from which the parallel region was started.
    @param ctc_string     Initialization string. May be ignored if \<pomp2_handle\> is already initialized.
 */
extern void
POMP2_Parallel_fork( POMP2_Region_handle* pomp2_handle,
                     int                  if_clause,
                     int                  num_threads,
                     POMP2_Task_handle*   pomp2_old_task,
                     const char           ctc_string[] );

/** Called after a parallel region.

    \e OpenMP \e 3.0: When a task exits a parallel region the variable
    \e pomp2_old_task (defined in the instrumented user code) holds
    the id of the task that entered the region. The value is stored in
    the adapter (in \e pomp2_current_task) to be made available for
    the measurement system for the following regions.

    @param pomp2_handle   The handle of the region.
    @param pomp2_old_task The task id of the region inside of which the parallel region was executed.
 */
extern void
POMP2_Parallel_join( POMP2_Region_handle* pomp2_handle,
                     POMP2_Task_handle    pomp2_old_task );


/** Called at the start of a section.
    @param pomp2_handle  The handle of the region.
    @param ctc_string   Initialization string. May be ignored if \<pomp2_handle\> is already initialized.
 */
extern void
POMP2_Section_begin( POMP2_Region_handle* pomp2_handle,
                     const char           ctc_string[] );

/** Called at the end of a section.
    @param pomp2_handle  The handle of the region.
 */
extern void
POMP2_Section_end( POMP2_Region_handle* pomp2_handle );

/** Called before a set of sections.
    @param pomp2_handle  The handle of the region.
    @param ctc_string   Initialization string. May be ignored if \<pomp2_handle\> is already initialized.
 */
extern void
POMP2_Sections_enter( POMP2_Region_handle* pomp2_handle,
                      const char           ctc_string[] );

/** Called after a set of sections.
    @param pomp2_handle  The handle of the region.
 */
extern void
POMP2_Sections_exit( POMP2_Region_handle* pomp2_handle );

/** Called at the start of a single region.
    @param pomp2_handle  The handle of the region.
 */
extern void
POMP2_Single_begin( POMP2_Region_handle* pomp2_handle );

/** Called at the end of a single region.
    @param pomp2_handle  The handle of the region.
 */
extern void
POMP2_Single_end( POMP2_Region_handle* pomp2_handle );

/** Called before a single region.
    @param pomp2_handle The handle of the region.
    @param ctc_string   Initialization string. May be ignored if
   \<pomp2_handle\> is already initialized.
 */
extern void
POMP2_Single_enter( POMP2_Region_handle* pomp2_handle,
                    const char           ctc_string[] );

/** Called after a single region.
    @param pomp2_handle  The handle of the region.
 */
extern void
POMP2_Single_exit( POMP2_Region_handle* pomp2_handle );

/** Called before a workshare region.
    @param pomp2_handle The handle of the region.
    @param ctc_string   Initialization string. May be ignored if
   \<pomp2_handle\> is already initialized.
 */
extern void
POMP2_Workshare_enter( POMP2_Region_handle* pomp2_handle,
                       const char           ctc_string[] );

/** Called after a workshare region.
    @param pomp2_handle  The handle of the region.
 */
extern void
POMP2_Workshare_exit( POMP2_Region_handle* pomp2_handle );

/** Called at the start of an ordered region.
    @param pomp2_handle  The handle of the region.
 */
extern void
POMP2_Ordered_begin( POMP2_Region_handle* pomp2_handle );

/** Called at the end of an ordered region.
    @param pomp2_handle  The handle of the region.
 */
extern void
POMP2_Ordered_end( POMP2_Region_handle* pomp2_handle );

/** Called before an ordered region.
    @param pomp2_handle The handle of the region.
    @param ctc_string   Initialization string. Ignored.
 */
extern void
POMP2_Ordered_enter( POMP2_Region_handle* pomp2_handle,
                     const char           ctc_string[] );

/** Called after an ordered region.
    @param pomp2_handle  The handle of the region.
 */
extern void
POMP2_Ordered_exit( POMP2_Region_handle* pomp2_handle );

/** \e OpenMP \e 3.0: When a task encounters a task construct it creates
    a new task. The task may be scheduled for later execution or
    executed immediately. In both cases the pomp-adapter assigns the
    id of the currently active task to \e pomp2_old_task which is
    defined in the instrumented user code.

    @param pomp2_handle   The handle of the region.
    @param pomp2_old_task Pointer to the task id in the instrumented user code
    @param pomp2_if       If an if clause is present on the task
                          directive this variable holds the evaluated
                          result of the argument of the if
                          clause. Else it is 1.
    @param ctc_string     The initialization string.

 */
extern void
POMP2_Task_create_begin( POMP2_Region_handle* pomp2_handle,
                         POMP2_Task_handle*   pomp2_new_task,
                         POMP2_Task_handle*   pomp2_old_task,
                         int                  pomp2_if,
                         const char           ctc_string[] );

/** \e OpenMP \e 3.0: After the code region which is executed as a
    separate task, the task id of the creating task is restored from
    \e pomp2_old_task (defined in the instrumented user code) and
    stored in \e pomp2_current_task inside the adapter.

    @param pomp2_handle   The region handle.
    @param pomp2_old_task The task id of the task inside of which the
                          task was created. This is stored inside the
                          instrumented user code.  */
extern void
POMP2_Task_create_end( POMP2_Region_handle* pomp2_handle,
                       POMP2_Task_handle    pomp2_old_task );

/** \e OpenMP \e 3.0: Marks the beginning of the execution of a task.

    @param pomp2_handle The region handle.
    @param pomp2_task handle of task.
 */
extern void
POMP2_Task_begin( POMP2_Region_handle* pomp2_handle,
                  POMP2_Task_handle    pomp2_task );

/** \e OpenMP \e 3.0: Marks the end of the execution of a task.

    @param pomp2_handle The region handle.
 */
extern void
POMP2_Task_end( POMP2_Region_handle* pomp2_handle );

/** \e OpenMP \e 3.0: When a task encounters a task construct it
    creates a new task. If the untied clause is specified the task is
    executed as an untied task. The task may be scheduled for later
    execution or executed immediately. In both cases the pomp-adapter
    assigns the id of the currently active task to \e pomp2_old_task
    which is defined in the instrumented user code.

    @param pomp2_handle   The handle of the region.
    @param pomp2_old_task Pointer to the task id in the instrumented
                          user code.
    @param pomp2_if       If an if clause is present on the task
                          directive this variable holds the evaluated
                          result of the argument of the if
                          clause. Else it is 1.
    @param ctc_string     The initialization string.

 */
extern void
POMP2_Untied_task_create_begin( POMP2_Region_handle* pomp2_handle,
                                POMP2_Task_handle*   pomp2_new_task,
                                POMP2_Task_handle*   pomp2_old_task,
                                int                  pomp2_if,
                                const char           ctc_string[] );

/** \e OpenMP \e 3.0: After the code region which is executed as a
    separate untied task, the task id of the creating task is restored
    from \e pomp2_old_task (defined in the instrumented user code) and
    stored in \e pomp2_current_task inside the adapter.

    @param pomp2_handle   The region handle.
    @param pomp2_old_task The id of the task from which the untied
                          task was created. This is stored in the
                          instrumented user code.
 */

extern void
POMP2_Untied_task_create_end( POMP2_Region_handle* pomp2_handle,
                              POMP2_Task_handle    pomp2_old_task );

/** \e OpenMp \e 3.0: Marks the beginning of the execution of an
    untied task.

    @param pomp2_handle      The region handle.
    @param pomp2_task Handle of this task.
 */
void
POMP2_Untied_task_begin( POMP2_Region_handle* pomp2_handle,
                         POMP2_Task_handle    pomp2_task );

/** \e OpenMP \e 3.0: Marks the end of the execution of a task.

    @param pomp2_handle The region handle.
 */
extern void
POMP2_Untied_task_end( POMP2_Region_handle* pomp2_handle );

/** Called before a taskwait. \n

    \e OpenMP \e 3.0: Taskwait directives can be used as scheduling
    points for tasks. When entering a taskwait region the task id of
    the currently executing task (\e pomp2_current_task) is saved in
    \e pomp2_old_task, which is defined inside the instrumented user
    code.

    @param pomp2_handle   The handle of the started region.
    @param pomp2_old_task Pointer to a "taskprivate" variable where
                          the current task id is stored.
    @param ctc_string     Initialization string. May be ignored if
   \<pomp2_handle\> is already initialized.
 */
extern void
POMP2_Taskwait_begin( POMP2_Region_handle* pomp2_handle,
                      POMP2_Task_handle*   pomp2_old_task,
                      const char           ctc_string[] );

/** Called after a barrier. \n

    \e OpenMP \e 3.0: When a task exits a taskwait region the variable
    \e pomp2_old_task (defined in the instrumented user code) holds
    the id of the task that entered the taskwait. The value is stored
    in the adapter (in \e pomp2_current_task) to be made available for
    the measurement system for the following regions.

    @param pomp2_handle   The handle of the ended region.
    @param pomp2_old_task "Taskprivate" variable storing the id of the
                          task the barrier is located in.
 */
extern void
POMP2_Taskwait_end( POMP2_Region_handle* pomp2_handle,
                    POMP2_Task_handle    pomp2_old_task );

/** Wraps the omp_get_max_threads function.
 *
 *  Needed for the instrumentation of parallel regions
 *  where the num_threads clause is used with the return
 *  value of the omp_get_max_threads function. This can't
 *  be used directly because the user may have declared
 *  it himself. Double declarations are not allowed. */
extern int
POMP2_Lib_get_max_threads( void );

/** Wraps the omp_init_lock function.
 * @param s The OpenMP lock to initialize.*/
extern void
POMP2_Init_lock( omp_lock_t* s );

/** Wraps the omp_destroy_lock function.
 * @param s The OpenMP lock to destroy. */
extern void
POMP2_Destroy_lock( omp_lock_t* s );

/** Wraps the omp_set_lock function.
 *  @param s The OpenMP lock to set. */
extern void
POMP2_Set_lock( omp_lock_t* s );

/** Wraps the omp_unset_lock function.
 *  @param s the OpenMP lock to unset. */
extern void
POMP2_Unset_lock( omp_lock_t* s );

/** Wraps the omp_test_lock function
*  @param s the OpenMP lock to test for.
*  @return result of omp_test_lock*/
extern int
POMP2_Test_lock( omp_lock_t* s );

/** Wraps the omp_init_nest_lock function.
 *  @param s The nested OpenMP lock to initialize. */
extern void
POMP2_Init_nest_lock( omp_nest_lock_t* s );

/** Wraps the omp_destroy_nest_lock function.
 *  @param s The nested OpenMP lock to destroy. */
extern void
POMP2_Destroy_nest_lock( omp_nest_lock_t* s );

/** Wraps the omp_set_nest_lock function
 *  @param s The nested OpenMP lock to set.*/
extern void
POMP2_Set_nest_lock( omp_nest_lock_t* s );

/** Wraps the omp_unset_nest_lock function
 *  @param s The nested OpenMP lock to unset.*/
extern void
POMP2_Unset_nest_lock( omp_nest_lock_t* s );

/** Wraps the omp_test_nest_lock function
*  @param s The nested OpenMP lock to test for.
*  @return result of omp_test_nest_lock*/
extern int
POMP2_Test_nest_lock( omp_nest_lock_t* s );

#endif /* _OPENMP */

#ifdef __cplusplus
}
#endif

#endif
