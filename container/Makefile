# SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
#
# SPDX-License-Identifier: Apache-2.0

CHIMERA_IMAGE ?= ghcr.io/pulp-platform/chimera:latest

SSH_PRIVATE_KEY ?= ~/.ssh/id_ed25519

.PHONY: chimera push clean help

help:
	@echo "Makefile for building the chimera-sdk Docker image"
	@echo ""
	@echo "Usage:"
	@echo "  make chimera   # Build the chimera-sdk image"
	@echo "  make push      # Push the image to the registry"
	@echo "  make clean     # Remove the local image"
	@echo ""
	@echo "Build Variables:"
	@echo "  CHIMERA_IMAGE    # Image name and tag (default: $(CHIMERA_IMAGE))"
	@echo "  SSH_PRIVATE_KEY  # SSH key with access to iis-git.ee.ethz.ch (default: $(SSH_PRIVATE_KEY))"
	@echo ""
	@echo "Example Usage:"
	@echo "  make chimera CHIMERA_IMAGE=ghcr.io/my-org/chimera:devel"
	@echo "  make chimera SSH_PRIVATE_KEY=~/.ssh/id_rsa"

# Build the chimera-sdk image (SSH agent needed for the internal LLVM repository)
chimera:
	eval $$(ssh-agent -s) && \
	ssh-add $(SSH_PRIVATE_KEY) && \
	DOCKER_BUILDKIT=1 docker build \
		--ssh default \
		-f Dockerfile \
		-t $(CHIMERA_IMAGE) \
		..

push:
	docker push $(CHIMERA_IMAGE)

clean:
	docker rmi $(CHIMERA_IMAGE) || true
