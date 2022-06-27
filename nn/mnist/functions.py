import matplotlib.pyplot as plt


def plot_lst(img_list):
    fig, ax = plt.subplots(nrows=1, ncols=len(img_list))
    for i, img in enumerate(img_list):
        plt.imshow(img)
        ax[i].imshow(img, cmap="gray")
    plt.show()
    
def test_model(indexes, model, image_transforms, mnist):
    images = []
    model_images = []
    for idx in indexes:
        image = image_transforms(mnist[idx][0])
        images.append(image.detach().numpy()[0])
        model_image = model(image)[1].detach().numpy()[0]
        model_images.append(model_image)
    plot_lst(images)
    plot_lst(model_images)
    
def train_test_model(model, optimizer, loss_fn, dataloader, mode="train"):
    if mode == "train":
        model.train()
    else:
        model.eval()
        
    total_loss = 0
    for images, labels in trainloader:
        if mode == "train":
            optimizer.zero_grad()
        _, model_images = model(images)
        loss = loss_fn(model_images, images)
        if mode == "train":
            loss.backward()
            optimizer.step()
            
        total_loss += loss.item()
        
    return model, optimizer, total_loss