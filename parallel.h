//
// Created by Rafiq on 2018-09-22.
//

#ifndef RENDERER_PARALLEL_H
#define RENDERER_PARALLEL_H

extern thread_local int ThreadIndex;
void ParallelInit();
void ParallelCleanup();
void ParallelFor(std::function<void(int)> func, int count, int chunkSize);
int NumSystemCores();


#endif //RENDERER_PARALLEL_H
