/*
 * Copyright (c) 2018, Mellanox Technologies. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __MLX5_EN_XDP_H__
#define __MLX5_EN_XDP_H__

#include "en.h"

#define MLX5E_XDP_MAX_MTU ((int)(PAGE_SIZE - \
				 MLX5_SKB_FRAG_SZ(XDP_PACKET_HEADROOM)))
#define MLX5E_XDP_MIN_INLINE (ETH_HLEN + VLAN_HLEN)
#define MLX5E_XDP_TX_DS_COUNT \
	((sizeof(struct mlx5e_tx_wqe) / MLX5_SEND_WQE_DS) + 1 /* SG DS */)

bool mlx5e_xdp_handle(struct mlx5e_rq *rq, struct mlx5e_dma_info *di,
		      void *va, u16 *rx_headroom, u32 *len);
bool mlx5e_poll_xdpsq_cq(struct mlx5e_cq *cq);
void mlx5e_free_xdpsq_descs(struct mlx5e_xdpsq *sq);

bool mlx5e_xmit_xdp_frame(struct mlx5e_rq *rq, struct mlx5e_dma_info *di,
			  const struct xdp_buff *xdp);

static inline void mlx5e_xmit_xdp_doorbell(struct mlx5e_xdpsq *sq)
{
	struct mlx5_wq_cyc *wq = &sq->wq;
	struct mlx5e_tx_wqe *wqe;
	u16 pi = mlx5_wq_cyc_ctr2ix(wq, sq->pc - 1); /* last pi */

	wqe  = mlx5_wq_cyc_get_wqe(wq, pi);

	mlx5e_notify_hw(wq, sq->pc, sq->uar_map, &wqe->ctrl);
}

#endif