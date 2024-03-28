/* Goal is to partition a mesh with $E$ elements on $E$ MPI processes i.e.,
 * one element for process. Mesh is a linear mesh i.e., $E$ elements are
 * placed one after the other so that each element has two neighbors apart
 * from the first and last elements (which only have 1 neighbor).
 * */

#include <stdbool.h>
#include <stdlib.h>

#include "parhip_interface.h"

void call_parhip(MPI_Comm comm) {
  int size, rank;
  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);

  if (size == 1) return;

  idxtype *vtxdist = malloc(sizeof(idxtype) * (size + 1));
  vtxdist[0] = 0;
  for (int i = 0; i < size; i++) vtxdist[i + 1] = vtxdist[i] + 1;

  int num_neighbors = 2;
  if (rank == 0 || rank == size - 1) num_neighbors = 1;

  idxtype *xadj = malloc(sizeof(idxtype) * (1 + 1));
  xadj[0] = 0, xadj[1] = num_neighbors;

  idxtype *adjncy = malloc(sizeof(idxtype) * num_neighbors);
  /* neighbor of the first element is the second element*/
  if (rank == 0) adjncy[0] = 1;
  /* neighbor of the last element is the second to last element */
  else if (rank == size - 1)
    adjncy[0] = size - 2;
  /* Otherwise, we have two neighbors: one to the left and one to the right */
  else
    adjncy[0] = rank - 1, adjncy[1] = rank + 1;

  idxtype *vwgt = malloc(sizeof(idxtype) * 1);
  vwgt[0] = 1;

  idxtype *adjwgt = malloc(sizeof(idxtype) * num_neighbors);
  for (int i = 0; i < num_neighbors; i++) adjwgt[i] = 1;

  int num_parts = size;
  double imbalance = 0.03;
  bool suppress_output = false;
  int seed = 0;
  int mode = ECOMESH;
  int edgecut = 0;
  idxtype *part = malloc(sizeof(idxtype) * 1);

  ParHIPPartitionKWay(vtxdist, xadj, adjncy, vwgt, adjwgt, &num_parts,
                      &imbalance, suppress_output, seed, mode, &edgecut, part,
                      &comm);

  free(vtxdist), free(xadj), free(adjncy);
  free(vwgt), free(adjwgt), free(part);
}

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  call_parhip(MPI_COMM_WORLD);

  MPI_Finalize();

  return 0;
}
