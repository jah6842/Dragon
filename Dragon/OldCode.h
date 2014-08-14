

/* STENCIL BUFFER USAGE / REFLECTIONS*/
glEnable(GL_STENCIL_TEST);

// Draw floor
glStencilFunc(GL_ALWAYS, 1, 0xFF);
glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
glStencilMask(0xFF);
glDepthMask(GL_FALSE);
glClear(GL_STENCIL_BUFFER_BIT);

glDrawArrays(GL_TRIANGLES, 36, 6);

// Draw cube reflection
glStencilFunc(GL_EQUAL, 1, 0xFF);
glStencilMask(0x00);
glDepthMask(GL_TRUE);

model = glm::scale(glm::translate(model, glm::vec3(0, 0, -1)), glm::vec3(1, 1, -1));
glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

GLint uniColor = glGetUniformLocation(basicProgram->GetHandle(), "overrideColor");
glUniform3f(uniColor, 0.3f, 0.3f, 0.3f);
glDrawArrays(GL_TRIANGLES, 0, 36);
glUniform3f(uniColor, 1.0f, 1.0f, 1.0f);

glDisable(GL_STENCIL_TEST);